#include "common/trace.h"

#include "profiler/region.h"

namespace Profiler {

    bool Region::Dispatch( const Event * e )
    {
        switch (mLastEnding)
        {
        case REGION_BEGIN:
            {
                if (e->ending == REGION_BEGIN)
                {
                    TRACE_ERROR("Wrong ending of the event");
                    return false;
                }

                if (mName == NULL)
                {
                    mName       = e->token->GetName();
                    mFileName   = e->token->GetFileName();
                    mLineNumber = e->token->GetLineNumber();
                }

                uint64_t period = e->time - mLastTime;
                mTotalTime  += period;
                mMinTime    =  (mMinTime > period) ? period : mMinTime;
                mMaxTime    =  (mMaxTime < period) ? period : mMaxTime;
                mLastEnding = e->ending;
            }
            break;

        case REGION_END:
            {
                if (e->ending == REGION_END)
                {
                    TRACE_ERROR("Wrong ending of the event");
                    return false;
                }

                mHits ++;
                mLastTime   = e->time;
                mLastEnding = e->ending;
            }
            break;

        default:
            TRACE_FAIL();
            return false;
        }

        return true;
    }

} // namespace Profiler
