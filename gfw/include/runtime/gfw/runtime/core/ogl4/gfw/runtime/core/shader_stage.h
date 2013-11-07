#ifndef __GFW_RUNTIME_CORE_SHADER_STAGE_H__
#define __GFW_RUNTIME_CORE_SHADER_STAGE_H__

#include "gfw/shared/types_fwd.h"

namespace GFW {

    uint32_t
    GetOGLShaderType( ShaderStage );

    uint32_t
    GetOGLShaderStageBit( ShaderStage );

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_SHADER_STAGE_H__
