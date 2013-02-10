#ifndef __PROFILER_MANAGER_H__
#define __PROFILER_MANAGER_H__

#include "common/typedefs.h"
#include "common/critical_section.h"

#include "profiler/event_bucket.h"

namespace Profiler {

    class Logger
    {
    public:
        void CreateEvent(const Token *, RegionEnding ending);

    public:
        Logger();
        ~Logger();

    private:
        void Process();
        void Dump();

    private:
        EventBucket *           mFirstBucket;
        EventBucket *           mLastBucket;

        Common::CritSec         mEventCriticalSection;

        const char *            mOutputFileName;


    };

    Logger * GetGlobalLogger();

} // namespace Profiler

#endif // __PROFILER_MANAGER_H__
