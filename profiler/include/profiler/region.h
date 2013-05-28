#ifndef __PROFILER_REGION_H__
#define __PROFILER_REGION_H__

#include "common/typedefs.h"
#include "profiler/token.h"
#include "profiler/event.h"

namespace Profiler {

    class Region
    {
    public:
        bool                    Dispatch(const Event *);

        inline RegionEnding     GetLastEnding() const   { return mLastEnding; }

        inline const char *     GetName() const         { return mName; }

        inline unsigned int     GetHits() const         { return mHits; }

        inline uint64_t         GetTotalTime() const    { return mTotalTime; }

        inline uint64_t         GetMinTime() const      { return mMinTime; }

        inline uint64_t         GetMaxTime() const      { return mMaxTime; }

    public:
        Region()
            : mName(NULL)
            , mFileName(NULL)
            , mLineNumber(0)
            , mLastEnding(REGION_END)
            , mLastTime(0)
            , mHits(0)
            , mTotalTime(0)
            , mMinTime(-1)
            , mMaxTime(0)
        {}

    private:

        void                    CalculateHash();

    private:

        // Identifiers

        const char *            mName;
        const char *            mFileName;
        uint32_t                mLineNumber;

        // State

        RegionEnding            mLastEnding;
        uint64_t                mLastTime;

        // Statistics

        uint32_t                mHits;
        uint64_t                mTotalTime;
        uint64_t                mMinTime;
        uint64_t                mMaxTime;

    };

} // namespace Profiler

#endif // __PROFILER_REGION_H__
