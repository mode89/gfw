#ifndef __GFW_BASE_PASS_H__
#define __GFW_BASE_PASS_H__

#include "gfw/types_fwd.h"

namespace GFW {

    class IPass
    {
    public:
        virtual void
        Dispatch() const = 0;

        virtual ConstIShaderRef
        GetShader( ShaderStage ) const = 0;

        virtual
        ~IPass() {}
    };

} // namespace GFW

#endif // __GFW_BASE_PASS_H__
