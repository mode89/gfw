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

    class TaskManager : public ITaskManager
    {
    public:
        virtual ITaskRef
        CreateTask(TaskProc);

        virtual void
        Run();

    public:
        void
        Push(TaskIn);

        TaskRef
        Pop();

    public:
        TaskManager();
        ~TaskManager();

    private:
        std::vector<HANDLE>     mWorkerThreads;
        std::queue<TaskRef>     mTaskQueue;
        Common::Futex           mTaskQueueMutex;
        Common::Futex           mEmptyQueueMutex;
    };
    AUTOREF_REFERENCE_DECLARATION(TaskManager);

} // namespace TaskMan

#endif // __TASKMAN_WIN_TASKMAN_H__
