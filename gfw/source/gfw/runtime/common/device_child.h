#ifndef __GFW_RUNTIME_COMMON_DEVICE_CHILD_H__
#define __GFW_RUNTIME_COMMON_DEVICE_CHILD_H__

#include "gfw/types_fwd.h"

namespace GFW {

    SHARED_PTR_FORWARD_TYPEDEFS( Device );

    template < class Base >
    class ADeviceChild : public Base
    {
    public:
        virtual IDeviceRef
        GetDevice() const;

    public:
        ADeviceChild( DeviceIn device );
        ~ADeviceChild();

    protected:
        std::weak_ptr< Device > mDevice;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_DEVICE_CHILD_H__
