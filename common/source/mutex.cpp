#include "common/mutex.h"
#include "common/mutex_impl.h"

namespace Common {

    Mutex::Mutex()
        : mImpl(NULL)
    {
        mImpl = new MutexImpl;
        InitializeCriticalSection(&mImpl->nativeHandle);
    }

    Mutex::~Mutex()
    {
        DeleteCriticalSection(&mImpl->nativeHandle);
        delete mImpl;
    }

    void Mutex::Lock()
    {
        EnterCriticalSection(&mImpl->nativeHandle);
    }

    void Mutex::Unlock()
    {
        LeaveCriticalSection(&mImpl->nativeHandle);
    }

} // namespace Common
