#ifndef __GFW_RUNTIME_CORE_SHADER_REFLECT_H__
#define __GFW_RUNTIME_CORE_SHADER_REFLECT_H__

#include "gfw/runtime/common/shader_reflect.h"

namespace GFW {

    class ShaderReflection : public AShaderReflection<IShaderReflection>
    {
    public:
        ShaderReflection(uint32_t program, DeviceIn );
        ~ShaderReflection();
    };
    SHARED_PTR_TYPEDEFS(ShaderReflection);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_SHADER_REFLECT_H__
