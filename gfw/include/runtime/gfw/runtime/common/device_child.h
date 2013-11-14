#ifndef __GFW_RUNTIME_COMMON_DEVICE_CHILD_H__
#define __GFW_RUNTIME_COMMON_DEVICE_CHILD_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    template < class Base >
    class ADeviceChild : public Base
    {
    public:
        inline virtual IDeviceRef
        GetDevice() const { return mDevice; }

    public:
        ADeviceChild(IDeviceRef device);
        ~ADeviceChild();

    protected:
        IDeviceRef  mDevice;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_DEVICE_CHILD_H__
