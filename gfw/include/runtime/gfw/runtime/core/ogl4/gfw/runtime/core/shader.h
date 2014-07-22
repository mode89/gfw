#ifndef __GFW_RUNTIME_CORE_SHADER_H__
#define __GFW_RUNTIME_CORE_SHADER_H__

#include "gfw/base/shader.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class Shader: public ADeviceChild<IShader>
    {
    public:
        virtual ShaderStage
        GetStage() const { return mStage; }

        virtual ConstIShaderReflectionRef
        GetReflection() const { return std::static_pointer_cast<IShaderReflection>( mReflection ); }

    public:
        bool
        Compile(const char * source);

        uint32_t
        GetHash() const { return mHash; }

        uint32_t
        GetHandle() const { return mHandle; }

    public:
        Shader( const void * shaderBinary, ShaderStage, DeviceIn );
        ~Shader();

    private:
        ShaderStage             mStage;
        uint32_t                mHandle;
        uint32_t                mHash;
        ShaderReflectionRef     mReflection;

#if PLAT_DEBUG
        char *                  mSource;
#endif
    };
    SHARED_PTR_TYPEDEFS(Shader);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_SHADER_H__
