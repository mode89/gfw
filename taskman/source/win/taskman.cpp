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

            IRunnableRef task;

            while (task = mTaskManager->Dequeue(), task.IsAttached())
            {
                task->Run();
            }
        }

    public:
        WorkerThread(TaskManagerIn taskManager)
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
            WorkerThread * workerThread = new WorkerThread(this);

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

} // namespace TaskMan
