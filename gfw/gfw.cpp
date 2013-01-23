#include "gfw\gfw.h"
#include "trace\trace.h"
#include "gfw\graphics\opengl\device.h"
#include "gfw\allocator.h"

namespace GFW {

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
