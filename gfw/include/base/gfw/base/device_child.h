#ifndef __GFW_BASE_DEVICE_CHILD_H__
#define __GFW_BASE_DEVICE_CHILD_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    class IDeviceChild
    {
    public:
        virtual IDeviceRef
        GetDevice() const = 0;

        virtual
        ~IDeviceChild() {}
    };
    SHARED_PTR_TYPEDEFS(IDeviceChild);
}

#endif // __GFW_BASE_DEVICE_CHILD_H__
