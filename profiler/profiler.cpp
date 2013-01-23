#include <common/platform.h>

#include "profiler.h"
#include "platform.h"

#define DEFAULT_PROFILER_DATA_FILE_NAME     "profiler.dat"
#define EVENT_BUCKET_SIZE                   1024

#include "types.h"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <list>
#include <map>

#include <stdio.h>
#include <string.h>

namespace Profiler {

    typedef std::vector<tEvent>         tEventVector;
    typedef std::map<RegionKey, Region> RegionMap;

    class Profiler
    {
    public:
        Profiler()
            : m_firstBucket(0)
            , m_lastBucket(0)
            , m_outFileName(DEFAULT_PROFILER_DATA_FILE_NAME)
        {
            m_firstBucket = m_lastBucket = new tEventBucket;

            m_eventCriticalSection = ::Profiler::CreateCriticalSection();
        }

        ~Profiler()
        {
            std::cout << "Processing profiled data..." << std::endl;

            Dump();

            ::Profiler::DeleteCriticalSection(m_eventCriticalSection);
        }

        inline void Event(const char * regionName, RegionEnding ending)
        {
            time_t t = ::Profiler::GetTimer();

            ::Profiler::EnterCriticalSection(m_eventCriticalSection);
            {
                // Reached the end of the bucket
                if (m_lastBucket->lastEvent == &m_lastBucket->end)
                {
                    m_lastBucket->nextBucket = new tEventBucket;

                    if (m_lastBucket->nextBucket != 0)
                    {
                        m_lastBucket = m_lastBucket->nextBucket;
                    }
                    // Cannot allocate a new bucket
                    else
                    {
                        Dump();
                    }
                }

                tEvent & e = *m_lastBucket->lastEvent;
                e.name     = regionName;
                e.time     = t;
                e.ending   = ending;

                m_lastBucket->lastEvent ++;
            }
            ::Profiler::LeaveCriticalSection(m_eventCriticalSection);
        }

        void Dump()
        {
            RegionMap regions;

            for (tEventBucket * bucket = m_firstBucket; bucket; bucket = bucket->nextBucket)
            {
                for (int i = 0; &bucket->events[i] != &bucket->end && &bucket->events[i] != bucket->lastEvent; ++ i)
                {
                    tEvent & e = bucket->events[i];

                    RegionMap::iterator it = regions.find(e.name);
                    if (it == regions.end())
                    {
                        if (e.ending == REGION_END)
                        {
                            std::cout << "Profiler error: " << "End-region event for an undefined region " << e.name << std::endl;
                            return;
                        }

                        regions[e.name] = Region(e.name, e.time);
                    }
                    else
                    {
                        Region & r = it->second;

                        if (r.GetLastEnding() == e.ending)
                        {
                            std::cout << "Profiler error: " << "Unexpected ending of the event for the region " << e.name << std::endl;
                            return;
                        }

                        r.Hit(e.time);
                    }
                }
            }

            FILE * profilerDataFile = fopen(m_outFileName, "w");

            double dFreq = static_cast<double>(::Profiler::GetTimerFrequency());

            // Output events
            fprintf(profilerDataFile, "Name\tHits\tTotal\tAvg\tMin\tMax\n");
            for (RegionMap::iterator it = regions.begin(); it != regions.end(); ++ it)
            {
                Region & r = it->second;

                if (r.GetLastEnding() == REGION_BEGIN)
                {
                    std::cout << "Profiler error: " << "The region '" << r.GetName() << "' hasn't ended" << std::endl;
                    return;
                }

                double total = static_cast<double>(r.GetTotalTime()) / dFreq;
                double min   = static_cast<double>(r.GetMinTime()) / dFreq;
                double max   = static_cast<double>(r.GetMaxTime()) / dFreq;

                fprintf(profilerDataFile, "%32s%16u%16lf%16lf%16lf%16lf\n",
                    r.GetName(),
                    r.GetHits(),
                    total,
                    total / r.GetHits(),
                    min,
                    max);
            }

            fclose(profilerDataFile);
        }

    private:
        tEventBucket *      m_firstBucket;
        tEventBucket *      m_lastBucket;

        tCriticalSection    m_eventCriticalSection;

        const char *        m_outFileName;
    };

    static Profiler sProfiler;

    void BeginRegion(const char * regionName)
    {
        sProfiler.Event(regionName, REGION_BEGIN);
    }

    void EndRegion(const char * regionName)
    {
        sProfiler.Event(regionName, REGION_END);
    }

    Region::Region(const char * name, time_t time)
        : m_name(name)
        , m_hits(1)
        , m_lastEnding(REGION_BEGIN)
        , m_lastTime(time)
        , m_totalTime(0)
        , m_minTime(0xFFFFFFFFFFFFFFF >> 1)
        , m_maxTime(0)
    { }

    Region::Region()
        : m_name(0)
        , m_hits(0)
        , m_lastEnding(REGION_END)
        , m_lastTime(0)
        , m_totalTime(0)
        , m_minTime(0)
        , m_maxTime(0)
    { }

    void Region::Hit(time_t time)
    {
        // The begining of the region
        if (m_lastEnding == REGION_END)
        {
            m_hits ++;
            m_lastTime   = time;
            m_lastEnding = REGION_BEGIN;
        }
        // The end of the region
        else
        {
            time_t period = time - m_lastTime;

            m_totalTime  += period;
            m_minTime    = (m_minTime > period) ? period : m_minTime;
            m_maxTime    = (m_maxTime < period) ? period : m_maxTime;

            m_lastTime   =  time;
            m_lastEnding =  REGION_END;
        }
    }

    RegionKey::RegionKey(const char * key)
        : m_key(key)
    { }

    bool RegionKey::operator < (const RegionKey & other) const
    {
        return strcmp(m_key, other.m_key) < 0;
    }

} // namespace Profiler
