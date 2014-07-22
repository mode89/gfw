#ifndef __GFW_RUNTIME_COMMON_DEVICE_CHILD_INL__
#define __GFW_RUNTIME_COMMON_DEVICE_CHILD_INL__

#include "gfw/base/device.h"
#include "gfw/runtime/common/device_child.h"

namespace GFW {

    template < class Base >
    ADeviceChild<Base>::ADeviceChild( DeviceIn device )
        : mDevice( device )
    {

    }

    template < class Base >
    ADeviceChild<Base>::~ADeviceChild()
    {

    }

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_DEVICE_CHILD_INL__
