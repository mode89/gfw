#ifndef __GFW_PLATFORM_WIN_PLATFORM_H__
#define __GFW_PLATFORM_WIN_PLATFORM_H__

#include "common\typedefs.h"

namespace GFW { namespace Platform {

    uint64_t GetPerformanceCounter();
    uint64_t GetPerformanceCounterFrequency();

}} // namespace GFW::Platform

#endif // __GFW_PLATFORM_WIN_PLATFORM_H__
