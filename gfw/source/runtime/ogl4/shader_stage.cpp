#include "common/trace.h"

#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/core/shader_stage.h"
#include "gfw/shared/shader_stage.h"

namespace GFW {

    uint32_t GetOGLShaderType(ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderStage::PIXEL:
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
        case ShaderStage::VERTEX:
            return GL_VERTEX_SHADER_BIT;
        case ShaderStage::PIXEL:
            return GL_FRAGMENT_SHADER_BIT;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

} // namespace GFW
