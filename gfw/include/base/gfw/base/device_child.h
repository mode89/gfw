#ifndef __GFW_BASE_DEVICE_CHILD_H__
#define __GFW_BASE_DEVICE_CHILD_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    class IDeviceChild : public Common::ARefCounted
    {
    public:
        virtual IDeviceRef
        GetDevice() const = 0;

        virtual
        ~IDeviceChild() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IDeviceChild);
}

#endif // __GFW_BASE_DEVICE_CHILD_H__
