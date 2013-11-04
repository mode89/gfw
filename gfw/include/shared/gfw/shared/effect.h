#ifndef __GFW_SHARED_EFFECT_H__
#define __GFW_SHARED_EFFECT_H__

#include "gfw/shared/technique.h"
#include "serialization/named_value.h"

namespace GFW {

    struct EffectDesc
    {
        uint32_t techniqueCount;

        EffectDesc()
            : techniqueCount( 0 )
        {}

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( techniqueCount );
        }
    };

    class EffectBinary : public Common::ARefCounted
    {
        typedef Common::AutoPointer< TechniqueBinaryRef > TechniqueBinaryVec;
        typedef Common::AutoPointer< ShaderBinaryRef > ShaderBinaryVec;
    public:
        EffectDesc          mDesc;

        TechniqueBinaryVec  mTechniques;

        uint32_t            mShaderCount;
        ShaderBinaryVec     mShaders;

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( mDesc );
            archive & NAMED_ARRAY( mTechniques.GetPointer(), mDesc.techniqueCount );
            archive & NAMED_VALUE( mShaderCount );
            archive & NAMED_ARRAY( mShaders.GetPointer(), mShaderCount );
        }
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBinary);

} // namespace GFW

#endif // __GFW_SHARED_EFFECT_H__
