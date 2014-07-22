#ifndef __GFW_RUNTIME_COMMON_SHADER_TABLE_H__
#define __GFW_RUNTIME_CONNOM_SHADER_TABLE_H__

#include "gfw/base/types_fwd.h"
#include "gfw/shared/types_fwd.h"

#include <unordered_map>

namespace GFW {

    typedef std::unordered_map< const ShaderBinary *, ConstIShaderRef > ShaderTable;

}; // namespace GFW

#endif // __GFW_RUNTIME_COMMON_SHADER_TABLE_H__
