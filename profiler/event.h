#ifndef __PROFILER_EVENT_H__
#define __PROFILER_EVENT_H__

#include "profiler/token.h"

namespace Profiler {

    enum RegionEnding
    {
        REGION_BEGIN = 0,
        REGION_END
    };

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
