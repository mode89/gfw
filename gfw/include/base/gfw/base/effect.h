#ifndef __GFW_BASE_EFFECT_H__
#define __GFW_BASE_EFFECT_H__

#include "gfw/base/device_child.h"
#include "gfw/base/types_fwd.h"
#include "gfw/shared/shader_stage.h"

namespace GFW {

    class IEffect : public IDeviceChild
    {
    public:
        virtual void
        Dispatch() = 0;

        virtual IShaderRef
        GetShader(ShaderStage) = 0;

        virtual
        ~IEffect() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IEffect);

} // namespace GFW

#endif // __GFW_BASE_EFFECT_H__
