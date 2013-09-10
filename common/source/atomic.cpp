#include "common/platform.h"
#include "common\atomic.h"

#if PLAT_WIN32

    #include <windows.h>

    namespace Common {

        void AtomicIncrement(int32_t & dst)
        {
            volatile LONG * d = static_cast<volatile LONG *>(static_cast<void *>(&dst));
            InterlockedIncrement(d);
        }

        void AtomicDecrement(int32_t & dst)
        {
            volatile LONG * d = static_cast<volatile LONG *>(static_cast<void *>(&dst));
            InterlockedDecrement(d);
        }

        void AtomicExchange(int32_t & dst, int32_t val)
        {
            volatile LONG * d = static_cast<volatile LONG *>(static_cast<void *>(&dst));
            InterlockedExchange(d, val);
        }

        void AtomicCompareExchange(int32_t & dst, int32_t exch, int32_t cmp)
        {
            volatile LONG * d = static_cast<volatile LONG *>(static_cast<void *>(&dst));
            InterlockedCompareExchange(d, exch, cmp);
        }

    } // namespace Common

#else

    #error Unrecognized platform

#endif // Selecting the platform
