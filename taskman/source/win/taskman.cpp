#include "common/trace.h"

#include "taskman/win/taskman.h"

namespace TaskMan {

    using namespace Common;

    static void SetThreadName(const char * name)
    {
#if defined(PLAT_DEBUG) && defined(PLAT_COMPILER_MSVC)

        #define MS_VC_EXCEPTION 0x406d1388

        struct THREADNAME_INFO
        {
            DWORD dwType;        // must be 0x1000
            LPCSTR szName;       // pointer to name (in same addr space)
            DWORD dwThreadID;    // thread ID (-1 caller thread)
            DWORD dwFlags;       // reserved for future use, most be zero
        };

        THREADNAME_INFO info;
        info.dwType     = 0x1000;
        info.szName     = name;
        info.dwThreadID = -1;
        info.dwFlags    = 0;

        __try
        {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD),
                (DWORD *)&info);
        }
        __except (EXCEPTION_CONTINUE_EXECUTION) {}

#endif // PLAT_DEBUG && PLAT_COMPILER_MSVC
    }

    static DWORD WINAPI WorkerThread(LPVOID data)
    {
        WorkerThreadDesc & desc = *static_cast<WorkerThreadDesc*>(data);
        TaskManagerRef taskManager = desc.taskManager;

#ifdef PLAT_DEBUG
        char threadName[32] = "TaskMan_Worker_";
        uint32_t threadNameLength = strlen(threadName);
        itoa(desc.id, threadName + threadNameLength, 10);
        SetThreadName(threadName);
#endif // PLAT_DEBUG

        // Work loop

        IRunnableRef task;

        while (task = taskManager->Dequeue(), task.IsAttached())
        {
            task->Run();
        }

        return 0;
    }

    TaskManager *           TaskManager::mInstance      = NULL;

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
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        uint32_t threadCnt = si.dwNumberOfProcessors;
        mWorkerThreads.resize(threadCnt);

        for (uint32_t i = 0; i < threadCnt; ++ i)
        {
            mWorkerThreads[i].taskManager = this;
            mWorkerThreads[i].id          = i;
            mWorkerThreads[i].handle = CreateThread(NULL, 0, WorkerThread, &mWorkerThreads[i], CREATE_SUSPENDED, 0);
        }
    }

    TaskManager::~TaskManager()
    {
        TRACE_ASSERT(mInstance != NULL);
        mInstance = NULL;
    }

    void TaskManager::Run()
    {
        for (uint32_t i = 0; i < mWorkerThreads.size(); ++ i)
        {
            DWORD result = ResumeThread(mWorkerThreads[i].handle);
            TRACE_ASSERT(result != -1);
        }

        // Wait for the worker threads

        std::vector<HANDLE> wtHandles;
        wtHandles.resize(mWorkerThreads.size());
        for (uint32_t i = 0; i < mWorkerThreads.size(); ++ i)
        {
            wtHandles[i] = mWorkerThreads[i].handle;
        }

        DWORD result = WaitForMultipleObjects(mWorkerThreads.size(), wtHandles.data(), TRUE, INFINITE);
        TRACE_ASSERT(result == WAIT_OBJECT_0);
    }

    void TaskManager::Enqueue(IRunnableIn task)
    {
        mMutexQueue.Lock();
        {
            mTaskQueue.push(task);
        }
        mMutexQueue.Unlock();
    }

    IRunnableRef TaskManager::Dequeue()
    {
        IRunnableRef task;

        mMutexQueue.Lock();
        {
            if (!mTaskQueue.empty())
            {
                task = mTaskQueue.front();
                mTaskQueue.pop();
            }
        }
        mMutexQueue.Unlock();

        return task;
    }

} // namespace TaskMan
