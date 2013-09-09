#include "common/event.h"

#include <windows.h>

namespace Common {

    struct EventImpl
    {
        CONDITION_VARIABLE  condVar;
        CRITICAL_SECTION    critSect;
        bool                flag;
    };

    Event::Event()
        : mImpl(NULL)
    {
        mImpl = new EventImpl;
        mImpl->flag = false;
        InitializeCriticalSection(&mImpl->critSect);
        InitializeConditionVariable(&mImpl->condVar);
    }

    Event::~Event()
    {
        DeleteCriticalSection(&mImpl->critSect);
        delete mImpl;
    }

    void Event::Wait()
    {
        EnterCriticalSection(&mImpl->critSect);

        SleepConditionVariableCS(
            &mImpl->condVar,
            &mImpl->critSect,
            INFINITE);

        LeaveCriticalSection(&mImpl->critSect);
    }

    void Event::Notify()
    {
        WakeAllConditionVariable(&mImpl->condVar);
    }

} // namespace Common
