#include "cmn/platform.h"
#include "cmn/counter.h"

#if CMN_WIN32

    #include <windows.h>

    namespace Cmn {

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

    } // namespace Cmn

#else

    #error Unrecognized platform

#endif
