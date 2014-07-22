#include "common/trace.h"
#include "gfw/base/shader_stage.h"
#include "gfw/runtime/common/shader_stage.h"

namespace GFW {

    const char * GetStageString(ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::VERTEX:
            return "VERTEX";
        case ShaderStage::PIXEL:
            return "PIXEL";
        default:
            TRACE_FAIL_MSG("Unknown stage");
        }

        return NULL;
    }

} // namespace GFW
