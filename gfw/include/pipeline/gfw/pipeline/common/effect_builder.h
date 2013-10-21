#ifndef __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__

#include "common/autoref.h"
#include "common/string_table.h"
#include "gfw/pipeline/common/effect_binary.h"

namespace GFW {

    class EffectBuilder : public Common::ARefCounted
    {
    public:
        EffectBinaryRef
        Build( const char * fileName, Common::StringTable & );
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBuilder);

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_EFFECT_BUILDER_H__
