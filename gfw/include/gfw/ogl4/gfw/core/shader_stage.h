#ifndef __GFW_CORE_SHADER_STAGE_H__
#define __GFW_CORE_SHADER_STAGE_H__

#include "common/typedefs.h"
#include "gfw/base/shader_stage.h"

namespace GFW {

    uint32_t
    GetOGLShaderType(ShaderStage);

    uint32_t
    GetOGLShaderStageBit(ShaderStage);

} // namespace GFW

#endif // __GFW_CORE_SHADER_STAGE_H__
