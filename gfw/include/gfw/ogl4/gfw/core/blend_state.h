#ifndef __GFW_CORE_BLEND_STATE_H__
#define __GFW_CORE_BLEND_STATE_H__

#include "gfw/base/blend_state.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class BlendState : public IBlendState
    {
    public:
        virtual void        Apply() const;

    public:
        BlendState(const BlendStateDesc &, uint32_t descHash);
        ~BlendState();

    private:
        BlendStateDesc      mDesc;
    };

} // namespace GFW

#endif // __GFW_CORE_BLEND_STATE_H__
