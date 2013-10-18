#ifndef __GFW_PIPELINE_COMMON_EFFECT_BINARY_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BINARY_H__

#include "common/autoref.h"

#include "gfw_pipeline/effect.h"

namespace GFW { namespace Pipeline {

    class EffectBinary : public Common::ARefCounted
    {
    public:
        const EffectDesc &
        GetDesc() { return mDesc; }

    private:
        EffectDesc  mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBinary);

}} // namespace GFW::Pipeline

#endif // __GFW_PIPELINE_COMMON_EFFECT_BINARY_H__
