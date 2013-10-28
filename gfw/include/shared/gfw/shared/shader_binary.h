#ifndef __GFW_SHARED_SHADER_BINARY_H__
#define __GFW_SHARED_SHADER_BINARY_H__

#include "common/autoref.h"

namespace GFW {

    struct ShaderDesc
    {
        uint32_t    inputCount;

        ShaderDesc()
            : inputCount( 0 )
        {}
    };

    template < class Archive >
    void Serialize( Archive & ar, const char * name, ShaderDesc & desc )
    {
        Serialize( ar, "inputCount", desc.inputCount );
    }

    class ShaderBinary : public Common::ARefCounted
    {
        typedef Common::AutoPointer<uint8_t> ByteRef;
    public:
        ShaderDesc  mDesc;
        uint32_t    mSize;  // Size if the data
        ByteRef     mData;  // Binary data
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBinary );

    template < class Archive >
    void Serialize( Archive & ar, const char * name, ShaderBinary & bin )
    {
        Serialize( ar, "desc", bin.mDesc );
        Serialize( ar, "size", bin.mSize );
        Serialize( ar, "data", bin.mData, bin.mSize );
    }

} // namespace GFW

#endif // __GFW_SHARED_SHADER_BINARY_H__
