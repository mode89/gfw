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
        : mAllocator(Common::GetDefaultAllocator())
        , mFirstBucket(0)
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
                mFirstBucket = mLastBucket = COMMON_NEW(mAllocator, EventBucket);
            }

            // Reached the end of the bucket
            if (mLastBucket->lastEvent == &mLastBucket->end)
            {
                mLastBucket->nextBucket = COMMON_NEW(mAllocator, EventBucket);

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
                        COMMON_DELETE(mAllocator, EventBucket, bucket);
                        bucket = nextBucket;
                    }

                    mFirstBucket = mLastBucket = COMMON_NEW(mAllocator, EventBucket);
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
