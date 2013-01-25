#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\allocator.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

    Device::Device(IAllocator * a)
        : ADevice(a)
    {

    }

    IContextRef Device::CreateContext(Platform::IWindowIn window)
    {
        return GFW_NEW(mAllocator, Context) (window, mAllocator);
    }

    void Device::Release()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    IDeviceRef Device::CreateInstance(IAllocator * a)
    {
        uint32_t funcsLoaded = LoadFunctions();
        TRACE_ASSERT(funcsLoaded);

        if (funcsLoaded)
        {
            return GFW_NEW(a, Device) (a);
        }

        return NULL;
    }

}} // namespace GFW::OpenGL
