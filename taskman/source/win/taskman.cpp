#include "common/trace.h"

#include "taskman/win/task.h"
#include "taskman/win/taskman.h"

#include <concrtrm.h>

namespace TaskMan {

    using namespace Common;

    static DWORD WINAPI WorkerThread(LPVOID data)
    {
        TaskManagerRef taskManager = static_cast<TaskManager*>(data);

        TaskRef task;

        while (task = taskManager->Pop(), task.IsAttached())
        {
            TaskProc taskProc = task->GetProc();
            taskProc(task->GetData());
        }

        return 0;
    }

    ITaskManagerRef GetInstance()
    {
        static ITaskManagerRef instance;

        if (instance.IsNull())
        {
            instance = new TaskManager();
        }

        return instance;
    }

    TaskManager::TaskManager()
    {
        uint32_t threadCnt = concurrency::GetProcessorCount();
        mWorkerThreads.resize(threadCnt);

        for (uint32_t i = 0; i < threadCnt; ++ i)
        {
            mWorkerThreads[i] = CreateThread(NULL, 0, WorkerThread, this, CREATE_SUSPENDED, 0);
        }
    }

    TaskManager::~TaskManager()
    {

    }

    ITaskRef TaskManager::CreateTask(TaskProc proc)
    {
        return new Task(proc, this);
    }

    void TaskManager::Run()
    {
        for (uint32_t i = 0; i < mWorkerThreads.size(); ++ i)
        {
            DWORD result = ResumeThread(mWorkerThreads[i]);
            TRACE_ASSERT(result != -1);
        }

        DWORD result = WaitForMultipleObjects(mWorkerThreads.size(), mWorkerThreads.data(), TRUE, INFINITE);
        TRACE_ASSERT(result == WAIT_OBJECT_0);
    }

    void TaskManager::Push(TaskIn task)
    {
        mTaskQueueMutex.Lock();
        mTaskQueue.push(task);
        mTaskQueueMutex.Unlock();
    }

    TaskRef TaskManager::Pop()
    {
        // Wait if there are no tasks
        // Get the next task and return it

        TRACE_FAIL_MSG("Not yet implemented.");
        return NULL;
    }

} // namespace TaskMan
