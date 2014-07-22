#ifndef __GFW_SHARED_TYPES_FWD_H__
#define __GFW_SHARED_TYPES_FWD_H__

#include "common/shared_ptr_typedefs.h"

#include <memory>

namespace GFW {

    class EffectBinary;
    class PassBinary;
    class ShaderBinary;
    class TechniqueBinary;

    struct EffectDesc;
    struct ShaderDesc;

    enum ShaderStage : int32_t;

} // namespace GFW

#endif // __GFW_SHARED_TYPES_FWD_H__
