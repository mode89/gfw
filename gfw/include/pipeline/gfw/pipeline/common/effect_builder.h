#ifndef __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__

#include "common/autoref.h"
#include "gfw/pipeline/common/effect_binary.h"

namespace GFW { namespace Pipeline {

    class EffectBuilder : public Common::ARefCounted
    {
    public:
        EffectBinaryRef
        Build(const char * fileName);
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBuilder);

}} // namespace GFW::Pipeline

#endif // __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
