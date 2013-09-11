#include "common/condition_variable.h"
#include "common/condition_variable_impl.h"
#include "common/mutex_impl.h"

#ifdef PLAT_COMPILER_MSVC

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

#elif defined(PLAT_COMPILER_MINGW)

namespace Common {

    ConditionVariable::ConditionVariable()
        : mImpl(NULL)
    {
        mImpl = new ConditionVariableImpl;
        ZeroMemory(mImpl, sizeof(ConditionVariableImpl));

        mImpl->nativeHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
        InitializeCriticalSection(&mImpl->mutexWaitersCount);
        mImpl->waitersCount = 0;
    }

    ConditionVariable::~ConditionVariable()
    {
        CloseHandle(mImpl->nativeHandle);
        DeleteCriticalSection(&mImpl->mutexWaitersCount);
        delete mImpl;
    }

    void ConditionVariable::Wait(Mutex & mutex)
    {
        EnterCriticalSection(&mImpl->mutexWaitersCount);
        mImpl->waitersCount ++;
        LeaveCriticalSection(&mImpl->mutexWaitersCount);

        LeaveCriticalSection(&mutex.mImpl->nativeHandle);
        WaitForSingleObject(mImpl->nativeHandle, INFINITE);
        EnterCriticalSection(&mutex.mImpl->nativeHandle);

        EnterCriticalSection(&mImpl->mutexWaitersCount);
        mImpl->waitersCount --;
        if (mImpl->waitersCount == 0)
        {
            ResetEvent(mImpl->nativeHandle);
        }
        LeaveCriticalSection(&mImpl->mutexWaitersCount);
    }

    void ConditionVariable::Notify()
    {
        EnterCriticalSection(&mImpl->mutexWaitersCount);
        if (mImpl->waitersCount > 0)
        {
            SetEvent(mImpl->nativeHandle);
        }
        LeaveCriticalSection(&mImpl->mutexWaitersCount);
    }

} // namespace Common

#else
#error Undefined platform
#endif
