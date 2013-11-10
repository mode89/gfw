#ifndef __GFW_BASE_EFFECT_H__
#define __GFW_BASE_EFFECT_H__

#include "gfw/base/device_child.h"
#include "gfw/base/types_fwd.h"
#include "gfw/shared/types_fwd.h"

namespace GFW {

    class IEffect : public IDeviceChild
    {
    public:
        virtual void
        Dispatch( uint32_t tech = 0, uint32_t pass = 0 ) = 0;

        virtual IShaderRef
        GetShader( ShaderStage, uint32_t tech = 0, uint32_t pass = 0 ) = 0;

        virtual
        ~IEffect() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IEffect);

} // namespace GFW

#endif // __GFW_BASE_EFFECT_H__
