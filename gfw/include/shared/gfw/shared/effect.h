#ifndef __GFW_SHARED_EFFECT_H__
#define __GFW_SHARED_EFFECT_H__

#include "gfw/shared/shader.h"
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
        typedef Common::AutoArray< TechniqueBinaryRef > TechniqueBinaryVec;
        typedef Common::AutoArray< ShaderBinaryRef > ShaderBinaryVec;

    public:
        EffectDesc                      mDesc;

        TechniqueBinaryVec              mTechniques;

        uint32_t                        mShaderCount;
        ShaderBinaryVec                 mShaders;

        Common::StringTableBinaryRef    mStringTable;

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( mDesc );
            archive & NAMED_ARRAY( mTechniques, mDesc.techniqueCount );
            archive & NAMED_VALUE( mShaderCount );
            archive & NAMED_ARRAY( mShaders, mShaderCount );
            archive & NAMED_VALUE( mStringTable );
        }
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBinary);

} // namespace GFW

#endif // __GFW_SHARED_EFFECT_H__
