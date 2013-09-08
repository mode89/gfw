#ifndef __TASKMAN_WIN_TASKMAN_H__
#define __TASKMAN_WIN_TASKMAN_H__

#include "common/autoref.h"
#include "common/mutex.h"
#include "taskman/taskman.h"

#include <queue>
#include <vector>

#include <windows.h>

namespace TaskMan {

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
        virtual void
        Enqueue(ITaskIn);

        virtual void
        Run();

    public:
        ITaskRef
        Dequeue();

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
        std::queue<ITaskRef>            mTaskQueue;
        Common::Mutex                   mMutexQueue;
        static PLAT_THREAD_LOCAL bool   mQueueLocked;
        // Common::Mutex                   mMutexEmptyQueue;

        friend class ITaskManager;
    };

} // namespace TaskMan

#endif // __TASKMAN_WIN_TASKMAN_H__
