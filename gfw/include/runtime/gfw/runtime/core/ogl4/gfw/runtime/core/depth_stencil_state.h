#ifndef __GFW_CORE_DEPTH_STENCIL_STATE_H__
#define __GFW_CORE_DEPTH_STENCIL_STATE_H__

#include "gfw/base/depth_stencil_state.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class DepthStencilState : public ADeviceChild<IDepthStencilState>
    {
    public:
        virtual const DepthStencilStateDesc &
        GetDesc()   { return mDesc; }

        virtual void
        Apply() const;

    public:
        DepthStencilState(const DepthStencilStateDesc &, uint32_t descHash, IDeviceRef);
        ~DepthStencilState();

    private:
        DepthStencilStateDesc   mDesc;
    };

} // namespace GFW

#endif // __GFW_CORE_DEPTH_STENCIL_STATE_H__
