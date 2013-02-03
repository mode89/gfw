#ifndef __PROFILER_EVENT_H__
#define __PROFILER_EVENT_H__

#include "profiler/region.h"

namespace Profiler {

    struct Event
    {
        const Token *   token;
        uint64_t        time;
        RegionEnding    ending;

        Event()
            : token(0)
            , time(0)
            , ending(REGION_END)
        {}
    };

} // namespace Profiler

#endif // __PROFILER_EVENT_H__
