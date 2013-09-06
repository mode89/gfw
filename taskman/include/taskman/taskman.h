#ifndef __TASKMAN_TASKMAN_H__
#define __TASKMAN_TASKMAN_H__

#include "common/autoref.h"
#include "taskman/task.h"

namespace TaskMan {

    typedef void (*TaskProc)(void * data);

    class ITaskManager : public Common::ARefCounted
    {
    public:
        virtual ITaskRef
        CreateTask(TaskProc) = 0;

        virtual void
        Run() = 0;

        virtual
        ~ITaskManager() {}
    };
    AUTOREF_REFERENCE_DECLARATION(ITaskManager);

    ITaskManagerRef GetInstance();

} // namespace TaskMan

#endif // __TASKMAN_TASKMAN_H__
