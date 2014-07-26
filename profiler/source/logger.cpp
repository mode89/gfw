#include "cmn/trace.h"
#include "cmn/counter.h"

#include "profiler/logger.h"
#include "profiler/config.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>

namespace Profiler {

    static Logger sGlobalLogger;

    Logger::Logger()
        : mFirstBucket(0)
        , mLastBucket(0)
        , mOutputFileName(DEFAULT_DATA_FILE_NAME)
    {
    }

    Logger::~Logger()
    {
        CMN_MSG( "Processing profile data..." );

        Process();
        Dump();
    }

    void Logger::CreateEvent(const Token * token, RegionEnding ending)
    {
        time_t t = Cmn::GetCounter();

        std::lock_guard< std::mutex > auto_lock( mMutex );

        if (mFirstBucket == NULL)
        {
            CMN_ASSERT( mLastBucket == NULL );
            mFirstBucket = mLastBucket = new EventBucket;
        }

        // Reached the end of the bucket
        if (mLastBucket->lastEvent == &mLastBucket->end)
        {
            mLastBucket->nextBucket = new EventBucket;

            if (mLastBucket->nextBucket != 0)
            {
                mLastBucket = mLastBucket->nextBucket;
            }
            // Cannot allocate a new bucket
            else
            {
                Process();

                // Deallocate dumped events
                for (EventBucket * bucket = mFirstBucket; bucket != NULL;)
                {
                    EventBucket * nextBucket = bucket->nextBucket;
                    delete bucket;
                    bucket = nextBucket;
                }

                mFirstBucket = mLastBucket = new EventBucket;
            }
        }

        Event * e  = mLastBucket->lastEvent;
        e->token   = token;
        e->time    = t;
        e->ending  = ending;

        mLastBucket->lastEvent ++;
    }

    void Logger::Dump()
    {
        std::ofstream fs;
        fs.open(mOutputFileName);

        if (fs.is_open() == true)
        {
            uint64_t freq = Cmn::GetCounterFrequency();

            fs.flags(std::ios::left);
            fs << std::setw(128) << "Region name" << " ";
            fs << std::setw(20) << "Hits" << " ";
            fs << std::setw(20) << "Total time" << " ";
            fs << std::setw(20) << "Average time" << " ";
            fs << std::setw(20) << "Minimum time" << " ";
            fs << std::setw(20) << "Maximum time";
            fs << std::endl << std::endl;

            for (RegionMap::iterator it = mRegions.begin(); it != mRegions.end(); ++ it)
            {
                const Region & r = it->second;

                uint64_t totalTime = r.GetTotalTime() * 1000000 / freq;
                uint64_t avgTime   = totalTime / r.GetHits();
                uint64_t minTime   = r.GetMinTime() * 1000000 / freq;
                uint64_t maxTime   = r.GetMaxTime() * 1000000 / freq;

                fs.flags(std::ios::left);
                fs << std::setw(128) << r.GetName() << " ";
                fs << std::setw(20) << r.GetHits() << " ";
                fs << std::setw(20) << totalTime << " ";
                fs << std::setw(20) << avgTime << " ";
                fs << std::setw(20) << minTime << " ";
                fs << std::setw(20) << maxTime;
                fs << std::endl;
            }

            fs.close();
        }
        else
        {
            TRACE_THROW("Cannot open %s", mOutputFileName);
        }
    }

    void Logger::Process()
    {
        for (EventBucket * bucket = mFirstBucket; bucket != NULL; bucket = bucket->nextBucket)
        {
            for (Event * e = &bucket->events[0]; e->token != NULL; ++ e)
            {
                mRegions[e->token->GetHash()].Dispatch(e);
            }
        }
    }

    Logger * GetGlobalLogger()
    {
        return &sGlobalLogger;
    }

} // namespace Profiler
