#include "common\atomic.h"
#include "common\platform.h"

#if PLATFORM_WIN32

#include <windows.h>

namespace Common {

    void AtomicIncrement(int32_t & val)
    {
        volatile LONG * v = static_cast<volatile LONG *>(static_cast<void *>(&val));
        InterlockedIncrement(v);
    }

    void AtomicDecrement(int32_t & val)
    {
        volatile LONG * v = static_cast<volatile LONG *>(static_cast<void *>(&val));
        InterlockedDecrement(v);
    }

} // namespace Common

#endif // Selecting the platform
