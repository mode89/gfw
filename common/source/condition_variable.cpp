#include "common/condition_variable.h"

#include <windows.h>

namespace Common {

    ConditionVariable::ConditionVariable()
        : mImpl(NULL)
    {

    }

    ConditionVariable::~ConditionVariable()
    {

    }

    void ConditionVariable::Wait(Mutex & mutex)
    {

    }

    void ConditionVariable::Notify()
    {

    }

} // namespace Common
