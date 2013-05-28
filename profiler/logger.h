#ifndef __PROFILER_MANAGER_H__
#define __PROFILER_MANAGER_H__

#include "common/typedefs.h"
#include "common/critical_section.h"

#include "profiler/event_bucket.h"
#include "profiler/region.h"

#include <map>

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
        typedef std::map< uint32_t, Region > RegionMap;

        EventBucket *           mFirstBucket;
        EventBucket *           mLastBucket;

        Common::CriticalSection mEventCriticalSection;

        const char *            mOutputFileName;

        RegionMap               mRegions;
    };

    Logger * GetGlobalLogger();

} // namespace Profiler

#endif // __PROFILER_MANAGER_H__