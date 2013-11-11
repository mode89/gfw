#ifndef __GFW_SHARED_TECHNIQUE_H__
#define __GFW_SHARED_TECHNIQUE_H__

#include "gfw/shared/pass.h"
#include "serialization/named_value.h"

#include <vector>

namespace GFW {

    struct TechniqueDesc
    {
        uint32_t passCount;

        TechniqueDesc()
            : passCount( 0 )
        {}

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( passCount );
        }
    };

    class TechniqueBinary : public Common::ARefCounted
    {
    public:
        typedef std::vector< PassBinaryRef > PassBinaryVec;

        TechniqueDesc                   mDesc;
        Common::InternedStringBinary    mName;
        PassBinaryVec                   mPasses;

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( mDesc );
            archive & NAMED_VALUE( mName );
        }
    };
    AUTOREF_REFERENCE_DECLARATION( TechniqueBinary );

} // namespace GFW

#endif // __GFW_SHARED_TECHNIQUE_H__
