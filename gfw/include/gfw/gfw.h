#ifndef __GFW_H__
#define __GFW_H__

#include "gfw\graphics\base\device.h"
#include "gfw\platform\platform.h"

namespace GFW {

    IDeviceRef CreateDevice(DeviceType deviceType = DEVICE_OPENGL);

} // namespace GFW

#endif // __GFW_H__
