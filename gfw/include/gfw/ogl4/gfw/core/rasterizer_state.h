#ifndef __GFW_CORE_RASTERIZER_STATE_H__
#define __GFW_CORE_RASTERIZER_STATE_H__

#include "gfw/base/rasterizer_state.h"
#include "gfw/common/device_child.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class RasterizerState : public ADeviceChild<IRasterizerState>
    {
    public:
        virtual void
        Apply() const;

    public:
        RasterizerState(const RasterizerStateDesc &, uint32_t descHash, IDeviceRef);
        ~RasterizerState();

    private:
        RasterizerStateDesc     mDesc;
    };

} // namespace GFW

#endif // __GFW_CORE_RASTERIZER_STATE_H__
