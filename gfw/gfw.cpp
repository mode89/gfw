#include "common/trace.h"

#include "gfw\gfw.h"
#include "gfw\graphics\opengl\device.h"
#include "gfw\allocator.h"

namespace GFW {

    using namespace Common;

    IDeviceRef CreateDevice(DeviceType deviceType)
    {
        IAllocator * defaultAllocator = GetDefaultAllocator();

        switch (deviceType)
        {
        case DEVICE_OPENGL:
            return OpenGL::Device::CreateInstance(defaultAllocator);

        default:
            TRACE_FAIL_MSG("Unknown device type");
        }

        return NULL;
    }

} // namespace GFW
