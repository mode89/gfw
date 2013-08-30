#ifndef __GFW_COMMON_EFFECT_H__
#define __GFW_COMMON_EFFECT_H__

#include "gfw/base/effect.h"
#include "gfw/base/types_fwd.h"

namespace GFW {

    class Effect : public IEffect
    {
    public:
        virtual void
        Dispatch(IContextRef);

    public:
        Effect();
        ~Effect();
    };
    AUTOREF_REFERENCE_DECLARATION(Effect);

} // namespace GFW

#endif // __GFW_COMMON_EFFECT_H__
