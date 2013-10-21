#ifndef __GFW_RUNTIME_CORE_SHADER_REFLECT_H__
#define __GFW_RUNTIME_CORE_SHADER_REFLECT_H__

#include "gfw/runtime/common/shader_reflect.h"

namespace GFW {

    class ShaderReflection : public AShaderReflection<IShaderReflection>
    {
    public:
        ShaderReflection(uint32_t program, IDeviceIn);
        ~ShaderReflection();
    };
    AUTOREF_REFERENCE_DECLARATION(ShaderReflection);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_SHADER_REFLECT_H__
