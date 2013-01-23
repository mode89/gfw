#include <windows.h>

#include "profiler/platform.h"

namespace Profiler {

    uint64_t GetTimer()
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return counter.QuadPart;
    }

    uint64_t GetTimerFrequency()
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return freq.QuadPart;
    }

    tCriticalSection CreateCriticalSection()
    {
        CRITICAL_SECTION * section = new CRITICAL_SECTION;
        ::InitializeCriticalSection(section);
        return section;
    }

    void DeleteCriticalSection(tCriticalSection section)
    {
        delete static_cast<CRITICAL_SECTION*>(section);
    }

    void EnterCriticalSection(tCriticalSection section)
    {
        ::EnterCriticalSection(static_cast<CRITICAL_SECTION *>(section));
    }

    void LeaveCriticalSection(tCriticalSection section)
    {
        ::LeaveCriticalSection(static_cast<CRITICAL_SECTION *>(section));
    }

} // namespace Profiler
