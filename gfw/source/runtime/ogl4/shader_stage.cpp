#include "common/trace.h"

#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/core/shader_stage.h"

namespace GFW {

    uint32_t GetOGLShaderType(ShaderStage stage)
    {
        switch (stage)
        {
        case SHADER_STAGE_VERTEX:
            return GL_VERTEX_SHADER;
        case SHADER_STAGE_PIXEL:
            return GL_FRAGMENT_SHADER;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

    uint32_t GetOGLShaderStageBit(ShaderStage stage)
    {
        switch (stage)
        {
        case SHADER_STAGE_VERTEX:
            return GL_VERTEX_SHADER_BIT;
        case SHADER_STAGE_PIXEL:
            return GL_FRAGMENT_SHADER_BIT;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

} // namespace GFW
