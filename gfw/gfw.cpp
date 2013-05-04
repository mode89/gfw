#include "common/trace.h"

#include "gfw/gfw.h"
#include "gfw/graphics/opengl/device.h"

namespace GFW {

    using namespace Common;

    IDeviceRef CreateDevice(DeviceType deviceType)
    {
        switch (deviceType)
        {
        case DEVICE_OPENGL:
            return OpenGL::Device::CreateInstance();

        default:
            TRACE_FAIL_MSG("Unknown device type");
        }

        return NULL;
    }

} // namespace GFW
