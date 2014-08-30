#ifndef __GFW_RUNTIME_CORE_SHADER_REFLECT_H__
#define __GFW_RUNTIME_CORE_SHADER_REFLECT_H__

#include "gfw/runtime/common/shader_reflect.h"
#include "gfw/shared/ogl4/shader.h"
#include <vector>

namespace GFW {

    struct TextureSamplerLocation
    {
        int32_t texture;    // texture register index
        int32_t sampler;    // sampler register index
        int32_t location;   // texture-sampler location

        TextureSamplerLocation()
            : texture( -1 )
            , sampler( -1 )
            , location( -1 )
        {}
    };

    typedef std::vector< TextureSamplerLocation > TextureSamplerLocationVec;

    class ShaderReflection : public AShaderReflection< IShaderReflection >
    {
    public:
        const TextureSamplerLocationVec &
        GetTextureSamplers() const { return mTextureSamplers; }

        size_t
        GetInputSignature() const { return mInputSignature; }

    public:
        ShaderReflection( const ShaderBinary &, const ShaderBinaryOgl4 &, uint32_t program, DeviceIn );
        ~ShaderReflection();

    private:
        TextureSamplerLocationVec   mTextureSamplers;
        size_t                      mInputSignature;
    };
    SHARED_PTR_TYPEDEFS( ShaderReflection );

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_SHADER_REFLECT_H__
