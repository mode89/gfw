#ifndef __GFW_SHARED_TECHNIQUE_BINARY_H__
#define __GFW_SHARED_TECHNIQUE_BINARY_H__

#include "common/autoref.h"

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
        TechniqueDesc   mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION( TechniqueBinary );

} // namespace GFW

#endif // __GFW_SHARED_TECHNIQUE_BINARY_H__
