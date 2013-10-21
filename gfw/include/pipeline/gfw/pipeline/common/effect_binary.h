#ifndef __GFW_PIPELINE_COMMON_EFFECT_BINARY_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BINARY_H__

#include "common/autoref.h"

#include "gfw/pipeline/common/effect.h"

namespace GFW {

    class EffectBinary : public Common::ARefCounted
    {
    public:
        const EffectDesc &
        GetDesc() { return mDesc; }

    private:
        EffectDesc  mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBinary);

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_EFFECT_BINARY_H__
