#include "common/event.h"
#include "common/platform.h"

#ifdef PLAT_COMPILER_MSVC

#include <windows.h>

namespace Common {

    struct EventImpl
    {
        CONDITION_VARIABLE  condVar;
        CRITICAL_SECTION    critSect;
    };

    Event::Event()
        : mImpl(NULL)
    {
        mImpl = new EventImpl;
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

#elif defined(PLAT_COMPILER_MINGW)

#include <windows.h>

namespace Common {

    struct EventImpl
    {
        HANDLE handle;
    };

    Event::Event()
        : mImpl(NULL)
    {
        mImpl = new EventImpl;
        mImpl->handle = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    Event::~Event()
    {
        CloseHandle(mImpl->handle);
        delete mImpl;
    }

    void Event::Wait()
    {
        WaitForSingleObject(mImpl->handle, INFINITE);
    }

    void Event::Notify()
    {
        SetEvent(mImpl->handle);
    }

} // namespace Common

#else
    #error Undefined platform
#endif
