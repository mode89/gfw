#ifndef __GFW_SHARED_SHADER_H__
#define __GFW_SHARED_SHADER_H__

#include "common/autoref.h"
#include "serialization/named_value.h"

namespace GFW {

    struct ShaderDesc
    {
        uint32_t    inputCount;

        ShaderDesc()
            : inputCount( 0 )
        {}

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( inputCount );
        }
    };

    class ShaderBinary : public Common::ARefCounted
    {
        typedef Common::AutoPointer<uint8_t> ByteArray;
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
