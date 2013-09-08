#include "common/mutex.h"

#include <windows.h>

namespace Common {

    Mutex::Mutex()
        : mImpl(NULL)
    {
        mImpl = new CRITICAL_SECTION;
        InitializeCriticalSection(static_cast<LPCRITICAL_SECTION>(mImpl));
    }

    Mutex::~Mutex()
    {
        DeleteCriticalSection(static_cast<LPCRITICAL_SECTION>(mImpl));
        delete static_cast<LPCRITICAL_SECTION>(mImpl);
    }

    void Mutex::Lock()
    {
        EnterCriticalSection(static_cast<LPCRITICAL_SECTION>(mImpl));
    }

    void Mutex::Unlock()
    {
        LeaveCriticalSection(static_cast<LPCRITICAL_SECTION>(mImpl));
    }

} // namespace Common
