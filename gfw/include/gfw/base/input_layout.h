#ifndef __GFW_BASE_INPUT_LAYOUT_H__
#define __GFW_BASE_INPUT_LAYOUT_H__

#include "gfw/base/device_child.h"
#include "gfw/base/types_fwd.h"

namespace GFW {

    class IInputLayout : public IDeviceChild
    {
    public:
        virtual
        ~IInputLayout() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IInputLayout);

} // namespace GFW

#endif // __GFW_BASE_INPUT_LAYOUT_H__
