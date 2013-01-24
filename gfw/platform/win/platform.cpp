#include "gfw\platform\win\platform.h"
#include "gfw\platform\win\window.h"
#include "gfw\allocator.h"

#include "trace\trace.h"

#include <windows.h>

namespace GFW { namespace Platform {

    uint64_t GetPerformanceCounterFrequency()
    {
        static uint64_t uFreq = 0;

        if (uFreq == 0)
        {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);
            uFreq = freq.QuadPart;
        }

        return uFreq;
    }

    uint64_t GetPerformanceCounter()
    {
        LARGE_INTEGER cnt;
        QueryPerformanceCounter(&cnt);
        return cnt.QuadPart;
    }

    IWindowRef CreateEmptyWindow(WindowDesc & desc)
    {
        return Window::CreateInstance(desc, GetDefaultAllocator());
    }

}} // namespace GFW::Platform
