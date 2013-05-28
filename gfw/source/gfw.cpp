#include "common/trace.h"

#include "gfw/gfw.h"
#include "gfw/graphics/opengl/device.h"

namespace GFW {

    using namespace Common;

    IDeviceRef CreateDevice(WindowHandle window, DeviceType deviceType)
    {
        switch (deviceType)
        {
        case DEVICE_OPENGL:
            return new OpenGL::Device(window);

        default:
            TRACE_FAIL_MSG("Unknown device type");
        }

        return NULL;
    }

} // namespace GFW
