#ifndef __GFW_RUNTIME_CORE_DEPTH_STENCIL_STATE_H__
#define __GFW_RUNTIME_CORE_DEPTH_STENCIL_STATE_H__

#include "gfw/depth_stencil_state.h"
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
        DepthStencilState( const DepthStencilStateDesc &, uint32_t descHash, DeviceIn );
        ~DepthStencilState();

    private:
        DepthStencilStateDesc   mDesc;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_DEPTH_STENCIL_STATE_H__
