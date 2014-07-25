#include "cmn/platform.h"
#include "cmn/counter.h"

#if PLAT_WIN32

    #include <windows.h>

    namespace Common {

        uint64_t GetCounter()
        {
            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);
            return static_cast< uint64_t >( counter.QuadPart );
        }

        uint64_t GetCounterFrequency()
        {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);
            return static_cast< uint64_t >( freq.QuadPart );
        }

    } // namespace Common

#else

    #error Unrecognized platform

#endif
