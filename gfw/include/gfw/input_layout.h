#ifndef __GFW_BASE_INPUT_LAYOUT_H__
#define __GFW_BASE_INPUT_LAYOUT_H__

#include "gfw/device_child.h"
#include "gfw/types_fwd.h"

namespace GFW {

    class IInputLayout : public IDeviceChild
    {
    public:
        virtual
        ~IInputLayout() {}
    };
    SHARED_PTR_TYPEDEFS(IInputLayout);

} // namespace GFW

#endif // __GFW_BASE_INPUT_LAYOUT_H__
