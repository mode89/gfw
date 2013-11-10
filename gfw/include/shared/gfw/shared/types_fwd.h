#ifndef __GFW_SHARED_TYPES_FWD_H__
#define __GFW_SHARED_TYPES_FWD_H__

#include "common/autoref.h"

namespace GFW {

    AUTOREF_FORWARD_DECLARATION( EffectBinary );
    AUTOREF_FORWARD_DECLARATION( PassBinary );
    AUTOREF_FORWARD_DECLARATION( ShaderBinary );
    AUTOREF_FORWARD_DECLARATION( TechniqueBinary );

    struct EffectDesc;
    struct ShaderDesc;

    enum ShaderStage : int32_t;

} // namespace GFW

#endif // __GFW_SHARED_TYPES_FWD_H__
