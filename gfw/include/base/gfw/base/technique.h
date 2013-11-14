#ifndef __GFW_BASE_TECHNIQUE_H__
#define __GFW_BASE_TECHNIQUE_H__

#include "gfw/base/pass.h"

namespace GFW {

    class ITechnique : public Common::ARefCounted
    {
    public:
        virtual void
        Dispatch( uint32_t pass = 0 ) = 0;

        virtual IShaderRef
        GetShader( ShaderStage, uint32_t pass = 0 ) const = 0;

        virtual
        ~ITechnique() {}
    };
    AUTOREF_REFERENCE_DECLARATION( ITechnique );

} // namespace GFW

#endif // __GFW_BASE_TECHNIQUE_H__
