#ifndef __GFW_PIPELINE_COMMON_EFFECT_H__
#define __GFW_PIPELINE_COMMON_EFFECT_H__

#include "gfw/shared/shader_stage.h"

namespace GFW {

    struct TechniqueDesc
    {
        uint32_t passCount;

        TechniqueDesc()
            : passCount( 0 )
        {}
    };

    struct PassDesc
    {
        uint32_t shaders[SHADER_STAGE_COUNT];
    };

} // namespace GFW::Pipeline

#endif // __GFW_PIPELINE_COMMON_EFFECT_H__
