#ifndef __GFW_CORE_BLEND_STATE_H__
#define __GFW_CORE_BLEND_STATE_H__

#include "gfw/base/blend_state.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class BlendState : public ADeviceChild<IBlendState>
    {
    public:
        virtual void
        Apply() const;

    public:
        BlendState(const BlendStateDesc &, uint32_t descHash, IDeviceRef);
        ~BlendState();

    private:
        BlendStateDesc      mDesc;
    };

} // namespace GFW

#endif // __GFW_CORE_BLEND_STATE_H__
