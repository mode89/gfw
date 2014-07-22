#ifndef __GFW_RUNTIME_CORE_PLATFORM_H__
#define __GFW_RUNTIME_CORE_PLATFORM_H__

#include "common/shared_ptr_typedefs.h"

#include <memory>

namespace GFW {

    SHARED_PTR_FORWARD_TYPEDEFS( IPlatform );
    class IPlatform
    {
    public:
        static  IPlatformRef
        Acquire();

        virtual
        ~IPlatform() { }
    };

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_PLATFORM_H__
