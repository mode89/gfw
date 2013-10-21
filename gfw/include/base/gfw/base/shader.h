#ifndef __GFW_BASE_SHADER_H__
#define __GFW_BASE_SHADER_H__

#include "gfw/base/device_child.h"
#include "gfw/base/shader_stage.h"
#include "gfw/base/types_fwd.h"

namespace GFW {

    class IShader: public IDeviceChild
    {
    public:
        virtual ShaderStage
        GetStage() = 0;

        virtual IShaderReflectionRef
        GetReflection() = 0;
    };
    AUTOREF_REFERENCE_DECLARATION(IShader);

} // namespace GFW

#endif // __GFW_BASE_SHADER_H__