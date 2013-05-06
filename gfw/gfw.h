#ifndef __GFW_H__
#define __GFW_H__

#include "gfw/common/window.h"
#include "gfw/graphics/base/device.h"

namespace GFW {

    IDeviceRef CreateDevice(WindowHandle, DeviceType deviceType = DEVICE_OPENGL);

} // namespace GFW

#endif // __GFW_H__
