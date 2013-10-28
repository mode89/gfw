#ifndef __GFW_SHARED_TECHNIQUE_H__
#define __GFW_SHARED_TECHNIQUE_H__

#include "gfw/shared/pass.h"

namespace GFW {

    struct TechniqueDesc
    {
        uint32_t passCount;

        TechniqueDesc()
            : passCount( 0 )
        {}
    };

    class TechniqueBinary : public Common::ARefCounted
    {
    public:
        typedef std::vector< PassBinaryRef > PassBinaryVec;

        TechniqueDesc           mDesc;
        Common::InternedString  mName;
        PassBinaryVec           mPasses;
    };
    AUTOREF_REFERENCE_DECLARATION( TechniqueBinary );

} // namespace GFW

#endif // __GFW_SHARED_TECHNIQUE_H__
