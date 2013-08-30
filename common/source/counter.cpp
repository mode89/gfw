#include "common/platform.h"
#include "common/counter.h"

#if PLAT_WIN32

    #include <windows.h>

    namespace Common {

        uint64_t GetCounter()
        {
            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);
            return counter.QuadPart;
        }

        uint64_t GetCounterFrequency()
        {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);
            return freq.QuadPart;
        }

    } // namespace Common

#else

    #error Unrecognized platform

#endif
