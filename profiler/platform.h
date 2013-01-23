#ifndef __PROFILER_PLATFORM_H__
#define __PROFILER_PLATFORM_H__

#include "common/typedefs.h"

namespace Profiler {

    typedef void *      tCriticalSection;

    uint64_t            GetTimer();
    uint64_t            GetTimerFrequency();

    tCriticalSection    CreateCriticalSection();
    void                DeleteCriticalSection(tCriticalSection);
    void                EnterCriticalSection(tCriticalSection);
    void                LeaveCriticalSection(tCriticalSection);

} // namespace Profiler

#endif // __PROFILER_PLATFORM_H__
