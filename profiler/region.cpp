#include "profiler/region.h"

#include "common/crc32.h"

namespace Profiler {

    using namespace Common;

    void Region::Hit(uint64_t time)
    {
        // The beginning of the region
        if (mLastEnding == REGION_END)
        {
            mHits ++;
            mLastTime   = time;
            mLastEnding = REGION_BEGIN;
        }
        // The end of the region
        else
        {
            uint64_t period = time - mLastTime;

            mTotalTime  += period;
            mMinTime    = (mMinTime > period) ? period : mMinTime;
            mMaxTime    = (mMaxTime < period) ? period : mMaxTime;

            mLastTime   =  time;
            mLastEnding =  REGION_END;
        }
    }

    bool Region::operator < ( const Region & other )
    {
        return mHash < other.mHash;
    }

} // namespace Profiler
