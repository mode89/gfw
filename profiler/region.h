#ifndef __PROFILER_REGION_H__
#define __PROFILER_REGION_H__

#include "common/typedefs.h"
#include "profiler/token.h"

namespace Profiler {

    enum RegionEnding
    {
        REGION_BEGIN = 0,
        REGION_END
    };

    class Region
    {
    public:

        bool                    operator < ( const Region & other );

        void                    Hit(uint64_t time);

        inline RegionEnding     GetLastEnding()     { return mLastEnding; }

        inline const char *     GetName()           { return mName; }

        inline unsigned int     GetHits()           { return mHits; }

        inline uint64_t         GetTotalTime()      { return mTotalTime; }

        inline uint64_t         GetMinTime()        { return mMinTime; }

        inline uint64_t         GetMaxTime()        { return mMaxTime; }

    private:

        void                    CalculateHash();

    private:

        // Identifiers

        const char *            mName;
        const char *            mFileName;
        uint32_t                mLineNumber;

        uint32_t                mHash;

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
