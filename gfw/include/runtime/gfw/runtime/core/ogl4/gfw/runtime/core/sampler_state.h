#ifndef __GFW_RUNTIME_CORE_SMAPLER_STATE_H__
#define __GFW_RUNTIME_CORE_SMAPLER_STATE_H__

#include "gfw/base/sampler_state.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class SamplerState : public ADeviceChild<ISamplerState>
    {
    public:
        virtual void
        Apply() const;

    public:
        SamplerState( const SamplerStateDesc & desc, uint32_t descHash, DeviceIn );
        ~SamplerState();

    private:
        SamplerStateDesc    mDesc;
        uint32_t            mHandle;
    };
    SHARED_PTR_TYPEDEFS(SamplerState);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_SAMPLER_STATE_H__
