#ifndef __GFW_CORE_PLATFORM_H__
#define __GFW_CORE_PLATFORM_H__

#include "common/autoref.h"

namespace GFW {

    AUTOREF_FORWARD_DECLARATION(IPlatform);
    class IPlatform: public Common::ARefCounted
    {
    public:
        static  IPlatformRef
        Acquire();

        virtual
        ~IPlatform() { }
    };

} // namespace GFW

#endif // __GFW_CORE_PLATFORM_H__
