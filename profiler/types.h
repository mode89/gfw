#ifndef __PROFILER_DEFS_H__
#define __PROFILER_DEFS_H__

#include "common/typedefs.h"

namespace Profiler {

    typedef long long  time_t;

    enum RegionEnding
    {
        REGION_BEGIN = 0,
        REGION_END
    };

    struct tEvent
    {
        const char *    name;
        time_t          time;
        RegionEnding    ending;

        tEvent()
            : name(0)
            , time(0)
            , ending(REGION_END)
        { }
    };

    struct tEventBucket
    {
        tEvent *        lastEvent;
        tEvent          events[EVENT_BUCKET_SIZE];
        tEvent          end;
        tEventBucket *  nextBucket;

        tEventBucket()
            : lastEvent(events)
            , nextBucket(0)
        { }
    };

    class Region
    {
    public:
        void                    Hit(time_t time);

        inline RegionEnding     GetLastEnding()     { return m_lastEnding; }
        inline const char *     GetName()           { return m_name; }
        inline unsigned int     GetHits()           { return m_hits; }
        inline time_t           GetTotalTime()      { return m_totalTime; }
        inline time_t           GetMinTime()        { return m_minTime; }
        inline time_t           GetMaxTime()        { return m_maxTime; }

    public:
        Region(const char * name, time_t time);
        Region();

    private:
        const char *    m_name;
        RegionEnding    m_lastEnding;
        time_t          m_lastTime;

        unsigned int    m_hits;
        time_t          m_totalTime;
        time_t          m_minTime;
        time_t          m_maxTime;
    };

    class RegionKey
    {
    public:
        RegionKey(const char * key);
        bool operator < (const RegionKey & other) const;

    private:
        const char * m_key;
    };

} // namespace Profiler

#endif // __PROFILER_DEFS_H__
