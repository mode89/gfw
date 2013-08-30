#ifndef __GFW_CORE_SMAPLER_STATE_H__
#define __GFW_CORE_SMAPLER_STATE_H__

#include "gfw/base/sampler_state.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class SamplerState : public ISamplerState
    {
    public:
        virtual void
        Apply();

    public:
        SamplerState(const SamplerStateDesc & desc, uint32_t descHash);
        ~SamplerState();

    private:
        SamplerStateDesc    mDesc;
        uint32_t            mHandle;
    };
    AUTOREF_REFERENCE_DECLARATION(SamplerState);

} // namespace GFW

#endif // __GFW_CORE_SAMPLER_STATE_H__
