#ifndef __GFW_RUNTIME_OGL4_SHADER_UTILS_H__
#define __GFW_RUNTIME_OGL4_SHADER_UTILS_H__

#include "gfw/types_fwd.h"

namespace GFW {

    uint32_t
    CompileShader( ShaderStage, const char * source );

    void
    LinkProgram( uint32_t program );

} // namespace GFW

#endif // __GFW_RUNTIME_OGL4_SHADER_UTILS_H__
