#ifndef __GFW_GRAPHICS_COMMON_DEVICE_H__
#define __GFW_GRAPHICS_COMMON_DEVICE_H__

#include "gfw\graphics\base\device.h"
#include "common\allocator.h"

namespace GFW {

    class ADevice: public IDevice
    {
    protected:
        ADevice(Common::IAllocator *);

    protected:
        Common::IAllocator *    mAllocator;
    };

} // namespace GFW

#endif // __GFW_GRAPHICS_COMMON_DEVICE_H__
