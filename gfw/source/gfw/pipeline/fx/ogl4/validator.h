#ifndef __GFW_PIPELINE_FX_OGL4_VALIDATOR_H__
#define __GFW_PIPELINE_FX_OGL4_VALIDATOR_H__

#include "gfw/shared/types_fwd.h"
#include <string>

namespace GFW {

    void
    AcquireValidator();

    void
    ReleaseValidator();

    void
    Validate( ShaderStage stage, const char * source );

} // namespace GFW

#endif // __GFW_PIPELINE_FX_OGL4_VALIDATOR_H__
