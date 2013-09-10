#include "common/condition_variable.h"
#include "common/condition_variable_impl.h"
#include "common/mutex_impl.h"

#include <windows.h>

namespace Common {

    ConditionVariable::ConditionVariable()
        : mImpl(NULL)
    {
        mImpl = new ConditionVariableImpl;
        ZeroMemory(mImpl, sizeof(ConditionVariableImpl));
    }

    ConditionVariable::~ConditionVariable()
    {
        delete mImpl;
    }

    void ConditionVariable::Wait(Mutex & mutex)
    {
        SleepConditionVariableCS(&mImpl->nativeHandle, &mutex.mImpl->nativeHandle, INFINITE);
    }

    void ConditionVariable::Notify()
    {
        WakeAllConditionVariable(&mImpl->nativeHandle);
    }

} // namespace Common
