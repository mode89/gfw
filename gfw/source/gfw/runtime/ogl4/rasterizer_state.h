#ifndef __GFW_RUNTIME_CORE_RASTERIZER_STATE_H__
#define __GFW_RUNTIME_CORE_RASTERIZER_STATE_H__

#include "gfw/rasterizer_state.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class RasterizerState : public ADeviceChild<IRasterizerState>
    {
    public:
        virtual void
        Apply() const;

    public:
        RasterizerState( const RasterizerStateDesc &, uint32_t descHash, DeviceIn );
        ~RasterizerState();

    private:
        RasterizerStateDesc     mDesc;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_RASTERIZER_STATE_H__
