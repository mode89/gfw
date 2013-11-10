#ifndef __GFW_SHARED_SHADER_H__
#define __GFW_SHARED_SHADER_H__

#include "common/autoref.h"
#include "gfw/shared/shader_stage.h"
#include "serialization/named_value.h"

namespace GFW {

    struct ShaderDesc
    {
        uint32_t    stage;
        uint32_t    variableCount;
        uint32_t    bufferCount;
        uint32_t    resourceCount;
        uint32_t    inputsCount;

        ShaderDesc()
            : stage( ShaderStage::UNKNOWN )
            , variableCount(0)
            , bufferCount(0)
            , resourceCount(0)
            , inputsCount(0)
        {}

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( stage );
            archive & NAMED_VALUE( variableCount );
            archive & NAMED_VALUE( bufferCount );
            archive & NAMED_VALUE( resourceCount );
            archive & NAMED_VALUE( inputsCount );
        }
    };

    class ShaderBinary : public Common::ARefCounted
    {
        typedef Common::AutoArray<uint8_t> ByteArray;
    public:
        ShaderDesc  mDesc;
        uint32_t    mSize;  // Size if the data
        ByteArray   mData;  // Binary data

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( mDesc );
            archive & NAMED_VALUE( mSize );
            archive & NAMED_ARRAY( mData, mSize );
        }
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBinary );

} // namespace GFW

#endif // __GFW_SHARED_SHADER_H__
