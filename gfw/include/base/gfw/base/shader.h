#ifndef __GFW_BASE_SHADER_H__
#define __GFW_BASE_SHADER_H__

#include "gfw/base/device_child.h"
#include "gfw/base/types_fwd.h"
#include "gfw/shared/types_fwd.h"

namespace GFW {

    class IShader: public IDeviceChild
    {
    public:
        virtual ShaderStage
        GetStage() const = 0;

        virtual ConstIShaderReflectionRef
        GetReflection() const = 0;

        virtual
        ~IShader() {}
    };
    SHARED_PTR_TYPEDEFS(IShader);

} // namespace GFW

#endif // __GFW_BASE_SHADER_H__
