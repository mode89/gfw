#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\allocator.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

    Device::Device(IPlatformIn p, IAllocator * a)
        : ADevice(a)
        , mPlatform(p)
    {

    }

    void Device::Release()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    IContextRef Device::CreateContext(Platform::IWindowIn window)
    {
        return GFW_NEW(mAllocator, Context) (window, this, mAllocator);
    }

    IDeviceRef Device::CreateInstance(IAllocator * a)
    {
        IPlatformRef platform = CreatePlatform(a);

        if (platform->Init())
        {
            return GFW_NEW(a, Device) (platform, a);
        }

        return NULL;
    }

}} // namespace GFW::OpenGL
