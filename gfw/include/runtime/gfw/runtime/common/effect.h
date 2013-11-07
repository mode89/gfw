#ifndef __GFW_RUNTIME_COMMON_EFFECT_H__
#define __GFW_RUNTIME_COMMON_EFFECT_H__

#include "gfw/base/effect.h"
#include "gfw/base/shader.h"
#include "gfw/base/types_fwd.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/shared/shader_stage.h"

namespace GFW {

    class Effect : public ADeviceChild<IEffect>
    {
    public:
        virtual void
        Dispatch();

        virtual IShaderRef
        GetShader(ShaderStage stage) { return mShaders[ static_cast<uint32_t>( stage ) ]; }

    public:
        Effect(IShaderRef[], IDeviceIn);
        ~Effect();

    private:
        IShaderRef  mShaders[ShaderStage::COUNT];
    };
    AUTOREF_REFERENCE_DECLARATION(Effect);

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_EFFECT_H__
