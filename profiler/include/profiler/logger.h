#ifndef __PROFILER_MANAGER_H__
#define __PROFILER_MANAGER_H__

#include "cmn/platform.h"
#include "profiler/event_bucket.h"
#include "profiler/region.h"

#include <map>

PLAT_WARNING_PUSH
PLAT_WARNING_DISABLE_MSVC( 4265 )
#include <mutex>
PLAT_WARNING_POP

namespace Profiler {

    class Logger
    {
    public:
        void CreateEvent(const Token *, RegionEnding ending);

    public:
        Logger();
        ~Logger();

        Logger( const Logger & );
        Logger & operator= ( const Logger & );

    private:
        void Process();
        void Dump();

    private:
        typedef std::map< uint64_t, Region > RegionMap;

        EventBucket *   mFirstBucket;
        EventBucket *   mLastBucket;

        std::mutex      mMutex;

        const char *    mOutputFileName;

        RegionMap       mRegions;
    };

    Logger * GetGlobalLogger();

} // namespace Profiler

#endif // __PROFILER_MANAGER_H__
