#include "common/trace.h"
#include "common/threads.h"

#include "threadpool/win/threadpool.h"

#include <windows.h>

namespace ThreadPool {

    using namespace Common;

    class WorkerThread : public IRunnable
    {
    public:
        virtual void Run()
        {
            // Work loop

            IRunnableRef task;

            while (task = mThreadPool->Dequeue(), task.IsAttached())
            {
                task->Run();
            }
        }

    public:
        WorkerThread(uint32_t id, ThreadPoolImplIn threadPool)
            : mThreadPool(threadPool)
        {}

    private:
        ThreadPoolImplRef   mThreadPool;
    };

    ThreadPoolImpl *   ThreadPoolImpl::mInstance      = NULL;

    IThreadPoolRef IThreadPool::GetInstance()
    {
        if (ThreadPoolImpl::mInstance == NULL)
        {
            ThreadPoolImpl::mInstance = new ThreadPoolImpl;
        }

        return ThreadPoolImpl::mInstance;
    }

    ThreadPoolImpl::ThreadPoolImpl()
        : mThreadCount(0)
        , mWaitersCount(0)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        mThreadCount = si.dwNumberOfProcessors;
    }

    ThreadPoolImpl::~ThreadPoolImpl()
    {
        TRACE_ASSERT(mInstance != NULL);
        mInstance = NULL;
    }

    void ThreadPoolImpl::Run()
    {
        mWaitersCount = 0;

        std::vector<IThreadRef> threads(mThreadCount);

        for (uint32_t i = 0; i < mThreadCount; ++ i)
        {
            WorkerThread * workerThread = new WorkerThread(i, this);

            const char * threadName = NULL;

#ifdef PLAT_DEBUG
            char name[32] = "ThreadPool_Worker_";
            uint32_t nameLen = strlen(name);
            itoa(i, name + nameLen, 10);
            threadName = name;
#endif // PLAT_DEBUG

            threads[i] = CreateThread(workerThread, threadName);
        }

        // Wait for the worker threads

        for (uint32_t i = 0; i < mThreadCount; ++ i)
        {
            threads[i]->Join();
        }
    }

    void ThreadPoolImpl::Enqueue(IRunnableIn task)
    {
        mMutexQueue.Lock();
        {
            mQueue.push(task);

            // Notify waiters that there is a new task on the queue
            if (mQueue.size() == 1)
            {
                mNotEmptyCondition.Notify();
            }
        }
        mMutexQueue.Unlock();
    }

    IRunnableRef ThreadPoolImpl::Dequeue()
    {
        IRunnableRef task;

        mMutexQueue.Lock();
        {
            while (mQueue.empty())
            {
                mWaitersCount ++;

                if (mWaitersCount < mThreadCount)
                {
                    mNotEmptyCondition.Wait(mMutexQueue);

                    if (mWaitersCount < mThreadCount)
                    {
                        mWaitersCount --;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    mNotEmptyCondition.Notify();
                    break;
                }
            }

            if (!mQueue.empty())
            {
                task = mQueue.front();
                mQueue.pop();
            }
        }
        mMutexQueue.Unlock();

        return task;
    }

} // namespace ThreadPool
