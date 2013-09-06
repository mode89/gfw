#ifndef __TASKMAN_TASK_H__
#define __TASKMAN_TASK_H__

#include "common/autoref.h"

namespace TaskMan {

    class ITask : public Common::ARefCounted
    {
    public:
        virtual void
        Run() = 0;

        virtual
        ~ITask() {}
    };
    AUTOREF_REFERENCE_DECLARATION(ITask);

} // namespace TaskMan

#endif // __TASKMAN_TASK_H__
