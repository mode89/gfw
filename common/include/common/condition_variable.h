#ifndef __COMMON_CONDITION_VARIABLE_H__
#define __COMMON_CONDITION_VARIABLE_H__

#include "common/mutex.h"

namespace Common {

    struct ConditionVariableImpl;

    class ConditionVariable
    {
    public:
        void
        Wait(Mutex &);

        void
        Notify();

    public:
        ConditionVariable();
        ~ConditionVariable();

    private:
        ConditionVariableImpl * mImpl;
    };

} // namespace Common

#endif // __COMMON_CONDITION_VARIABLE_H__
