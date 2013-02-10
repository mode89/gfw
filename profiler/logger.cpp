#include "common/trace.h"
#include "common/counter.h"

#include "profiler/logger.h"
#include "profiler/config.h"

#include <iostream>
#include <vector>
#include <algorithm>

namespace Profiler {

    static Logger sGlobalLogger;

    Logger::Logger()
        : mFirstBucket(0)
        , mLastBucket(0)
        , mOutputFileName(DEFAULT_DATA_FILE_NAME)
    {
        mEventCriticalSection = Common::CreateCriticalSection();
    }

    Logger::~Logger()
    {
        std::cout << "Processing profiled data..." << std::endl;

        Dump();

        Common::DeleteCriticalSection(mEventCriticalSection);
    }

    void Logger::CreateEvent(const Token * token, RegionEnding ending)
    {
        time_t t = Common::GetCounter();

        Common::EnterCriticalSection(mEventCriticalSection);
        {
            if (mFirstBucket == NULL)
            {
                TRACE_ASSERT(mLastBucket == NULL);
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
        Common::LeaveCriticalSection(mEventCriticalSection);
    }

    void Logger::Dump()
    {

    }

    void Logger::Process()
    {

    }

    Logger * GetGlobalLogger()
    {
        return &sGlobalLogger;
    }

} // namespace Profiler
