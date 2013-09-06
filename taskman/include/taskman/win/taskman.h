#ifndef __TASKMAN_WIN_TASKMAN_H__
#define __TASKMAN_WIN_TASKMAN_H__

#include "common/autoref.h"
#include "common/futex.h"
#include "taskman/taskman.h"

#include <queue>
#include <vector>

#include <windows.h>

namespace TaskMan {

    AUTOREF_FORWARD_DECLARATION(Task);
    AUTOREF_FORWARD_DECLARATION(TaskManager);

    struct WorkerThreadDesc
    {
        HANDLE          handle;
        uint32_t        id;
        TaskManager *   taskManager;
    };

    class TaskManager : public ITaskManager
    {
    public:
        virtual ITaskRef
        CreateTask(TaskProc);

        virtual void
        Run();

    public:
        void
        EnqueueTask(TaskIn);

        TaskRef
        DequeueTask();

    public:
        TaskManager();
        ~TaskManager();

    private:
        void
        LockQueue();

        void
        UnlockQueue();

    private:
        static TaskManager *            mInstance;

        std::vector<WorkerThreadDesc>   mWorkerThreads;
        std::queue<TaskRef>             mTaskQueue;
        Common::Futex                   mMutexQueue;
        static PLAT_THREAD_LOCAL bool   mQueueLocked;

        friend class ITaskManager;
    };

} // namespace TaskMan

#endif // __TASKMAN_WIN_TASKMAN_H__
