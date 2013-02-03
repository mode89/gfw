#ifndef __PROFILER_MANAGER_H__
#define __PROFILER_MANAGER_H__

#include "common/typedefs.h"
#include "common/critical_section.h"

#include "profiler/event_bucket.h"

namespace Profiler {

    class Manager
    {
    public:
        void CreateEvent(const Token *, RegionEnding ending);

    public:
        Manager();
        ~Manager();

    private:
        void Dump();

    private:
        EventBucket *   m_firstBucket;
        EventBucket *   m_lastBucket;

        Common::CritSec  m_eventCriticalSection;

        const char *    m_outFileName;
    };

    Manager * GetGlobalManager();

} // namespace Profiler

#endif // __PROFILER_MANAGER_H__
