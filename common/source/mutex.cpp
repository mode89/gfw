#include "common/mutex.h"

#include <windows.h>

namespace Common {

    struct MutexImpl
    {
        CRITICAL_SECTION critSect;
    };

    Mutex::Mutex()
        : mImpl(NULL)
    {
        mImpl = new MutexImpl;
        InitializeCriticalSection(&mImpl->critSect);
    }

    Mutex::~Mutex()
    {
        DeleteCriticalSection(&mImpl->critSect);
        delete mImpl;
    }

    void Mutex::Lock()
    {
        EnterCriticalSection(&mImpl->critSect);
    }

    void Mutex::Unlock()
    {
        LeaveCriticalSection(&mImpl->critSect);
    }

} // namespace Common
