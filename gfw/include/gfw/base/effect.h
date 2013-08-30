#ifndef __GFW_BASE_EFFECT_H__
#define __GFW_BASE_EFFECT_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    class IEffect : public Common::ARefCounted
    {
    public:
        virtual void
        Dispatch(IContextRef) = 0;

        virtual
        ~IEffect() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IEffect);

} // namespace GFW

#endif // __GFW_BASE_EFFECT_H__
