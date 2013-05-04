#ifndef __PROFILER_EVENT_BUCKET_H__
#define __PROFILER_EVENT_BUCKET_H__

#include "profiler/config.h"
#include "profiler/event.h"

namespace Profiler {

    struct EventBucket
    {
        Event *         lastEvent;
        Event           events[BUCKET_SIZE];
        Event           end;
        EventBucket *   nextBucket;

        EventBucket()
            : lastEvent(events)
            , nextBucket(0)
        {}
    };

} // namespace Profiler

#endif // __PROFILER_EVENT_BUCKET_H__
