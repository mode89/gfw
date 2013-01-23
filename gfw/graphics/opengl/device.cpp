#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\allocator.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    GFW::IContextRef Device::CreateContext( GFW::Platform::IWindowIn window )
    {
        return GFW_NEW(Context) (window);
    }

    void Device::Release()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

}} // namespace GFW::OpenGL
