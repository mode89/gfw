#include "common/trace.h"

#include "gfw/common/shader_stage.h"

namespace GFW {

    const char * GetStageString(ShaderStage stage)
    {
        switch (stage)
        {
        case SHADER_STAGE_VERTEX:
            return "VERTEX";
        case SHADER_STAGE_PIXEL:
            return "PIXEL";
        default:
            TRACE_FAIL_MSG("Unknown stage");
        }

        return NULL;
    }

} // namespace GFW
