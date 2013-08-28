#ifndef __GFW_COMMON_DEVICE_CHILD_INL__
#define __GFW_COMMON_DEVICE_CHILD_INL__

#include "gfw/base/device.h"

namespace GFW {

    template < class Base >
    ADeviceChild<Base>::ADeviceChild(IDeviceRef device)
        : mDevice(device)
    {

    }

    template < class Base >
    ADeviceChild<Base>::~ADeviceChild()
    {

    }

} // namespace GFW

#endif // __GFW_COMMON_DEVICE_CHILD_INL__
