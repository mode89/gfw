#ifndef __GFW_RUNTIME_CORE_SHADER_H__
#define __GFW_RUNTIME_CORE_SHADER_H__

#include "gfw/base/shader.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class Shader: public ADeviceChild<IShader>
    {
    public:
        virtual IShaderReflectionRef
        GetReflection() { return mReflection; }

    public:
        bool
        Compile(const char * source);

        inline uint32_t
        GetHash() { return mHash; }

        inline uint32_t
        GetHandle() { return mHandle; }

    public:
        Shader( ShaderBinaryRef, IDeviceRef );
        ~Shader();

    private:
        uint32_t                mHandle;
        uint32_t                mHash;
        ShaderReflectionRef     mReflection;
    };
    AUTOREF_REFERENCE_DECLARATION(Shader);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_SHADER_H__
