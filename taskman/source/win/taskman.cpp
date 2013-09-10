#include "common/trace.h"
#include "common/threads.h"

#include "taskman/win/taskman.h"

#include <windows.h>

namespace TaskMan {

    using namespace Common;

    class WorkerThread : public IRunnable
    {
    public:
        virtual void Run()
        {
            // Work loop

            IRunnableRef task;

            while (task = mTaskManager->Dequeue(), task.IsAttached())
            {
                task->Run();
            }
        }

    public:
        WorkerThread(uint32_t id, TaskManagerIn taskManager)
            : mTaskManager(taskManager)
        {}

    private:
        TaskManagerRef  mTaskManager;
    };

    TaskManager *   TaskManager::mInstance      = NULL;

    ITaskManagerRef ITaskManager::GetInstance()
    {
        if (TaskManager::mInstance == NULL)
        {
            TaskManager::mInstance = new TaskManager;
        }

        return TaskManager::mInstance;
    }

    TaskManager::TaskManager()
        : mThreadCount(0)
        , mWaitersCount(0)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        mThreadCount = si.dwNumberOfProcessors;
    }

    TaskManager::~TaskManager()
    {
        TRACE_ASSERT(mInstance != NULL);
        mInstance = NULL;
    }

    void TaskManager::Run()
    {
        mWaitersCount = 0;

        std::vector<IThreadRef> threads(mThreadCount);

        for (uint32_t i = 0; i < mThreadCount; ++ i)
        {
            WorkerThread * workerThread = new WorkerThread(i, this);

            const char * threadName = NULL;

#ifdef PLAT_DEBUG
            char name[32] = "TaskMan_Worker_";
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

    void TaskManager::Enqueue(IRunnableIn task)
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

    IRunnableRef TaskManager::Dequeue()
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

} // namespace TaskMan
