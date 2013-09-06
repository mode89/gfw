#ifndef __TASKMAN_TASKMAN_H__
#define __TASKMAN_TASKMAN_H__

#include "common/autoref.h"
#include "taskman/task.h"

namespace TaskMan {

    AUTOREF_FORWARD_DECLARATION(ITaskManager);
    class ITaskManager : public Common::ARefCounted
    {
    public:
        static ITaskManagerRef
        GetInstance();

        virtual void
        Enqueue(ITaskIn) = 0;

        virtual void
        Run() = 0;

        virtual
        ~ITaskManager() {}
    };

} // namespace TaskMan

#endif // __TASKMAN_TASKMAN_H__
