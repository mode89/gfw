#include "common/trace.h"
#include "common/threads.h"

#include "taskman/win/taskman.h"

namespace TaskMan {

    using namespace Common;

    class WorkerThread : public IRunnable
    {
    public:
        virtual void Run()
        {
            // Work loop

            static uint32_t activityMask = 0;
            static Mutex    mutexMask;

            while (true)
            {
                mutexMask.Lock();
                activityMask |= mMask;
                mutexMask.Unlock();

                IRunnableRef task;

                while (task = mTaskManager->Dequeue(), task.IsAttached())
                {
                    task->Run();
                }

                bool lastThread = false;

                mutexMask.Lock();
                activityMask &= ~mMask;
                if (activityMask == 0) lastThread = true;
                mutexMask.Unlock();

                if (!lastThread)
                {
                    mTaskManager->WaitNewTasks();

                    mutexMask.Lock();
                    activityMask |= mMask;
                    mutexMask.Unlock();
                }
                else
                {
                    mTaskManager->UnblockWaiters();
                    break;
                }
            }
        }

    public:
        WorkerThread(uint32_t id, TaskManagerIn taskManager)
            : mTaskManager(taskManager)
            , mMask(1 << id)
        {}

    private:
        TaskManagerRef  mTaskManager;
        uint32_t        mMask;
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
    {

    }

    TaskManager::~TaskManager()
    {
        TRACE_ASSERT(mInstance != NULL);
        mInstance = NULL;
    }

    void TaskManager::Run()
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        uint32_t threadCount = si.dwNumberOfProcessors;
        std::vector<IThreadRef> threads(threadCount);

        for (uint32_t i = 0; i < threadCount; ++ i)
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

        for (uint32_t i = 0; i < threadCount; ++ i)
        {
            threads[i]->Join();
        }
    }

    void TaskManager::Enqueue(IRunnableIn task)
    {
        mMutexQueue.Lock();
        {
            mQueue.push(task);

            if (mQueue.size() == 1)
            {
                UnblockWaiters();
            }
        }
        mMutexQueue.Unlock();
    }

    IRunnableRef TaskManager::Dequeue()
    {
        IRunnableRef task;

        mMutexQueue.Lock();
        {
            if (!mQueue.empty())
            {
                task = mQueue.front();
                mQueue.pop();
            }
        }
        mMutexQueue.Unlock();

        return task;
    }

    void TaskManager::WaitNewTasks()
    {
        mEventNewTask.Wait();
    }

    void TaskManager::UnblockWaiters()
    {
        mEventNewTask.Notify();
    }

} // namespace TaskMan
