#include "cmn/trace.h"

#include "gfw/shader_stage.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/shader_stage.h"

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
            CMN_FAIL();
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
            CMN_FAIL();
        }

        return 0;
    }

} // namespace GFW
