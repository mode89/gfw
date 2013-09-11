#ifndef __TASKMAN_WIN_TASKMAN_H__
#define __TASKMAN_WIN_TASKMAN_H__

#include "common/autoref.h"
#include "common/condition_variable.h"
#include "common/mutex.h"
#include "taskman/taskman.h"

#include <queue>

namespace TaskMan {

    class TaskManager : public ITaskManager
    {
    public:
        virtual void
        Enqueue(Common::IRunnableIn);

        virtual void
        Run();

    public:
        Common::IRunnableRef
        Dequeue();

    public:
        TaskManager();
        ~TaskManager();

    private:
        static TaskManager *                mInstance;

        uint32_t                            mThreadCount;

        std::queue<Common::IRunnableRef>    mQueue;
        Common::Mutex                       mMutexQueue;

        Common::ConditionVariable           mNotEmptyCondition;
        uint32_t                            mWaitersCount;

        friend class ITaskManager;
    };
    AUTOREF_REFERENCE_DECLARATION(TaskManager);

} // namespace TaskMan

#endif // __TASKMAN_WIN_TASKMAN_H__
