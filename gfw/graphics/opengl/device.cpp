#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\graphics\opengl\platform.h"
#include "gfw\allocator.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    Device::Device()
    {

    }

    GFW::IContextRef Device::CreateContext( GFW::Platform::IWindowIn window )
    {
        return GFW_NEW(Context) (window);
    }

    void Device::Release()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    IDeviceRef Device::CreateInstance()
    {
        uint32_t funcsLoaded = LoadFunctions();
        TRACE_ASSERT(funcsLoaded);

        if (funcsLoaded)
        {
            return GFW_NEW(Device);
        }

        return NULL;
    }

}} // namespace GFW::OpenGL
