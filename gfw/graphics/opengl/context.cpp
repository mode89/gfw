#include "gfw\graphics\opengl\context.h"
#include "gfw\graphics\opengl\platform.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

    Context::Context(Platform::IWindowIn window, IAllocator * a)
        : AContext(a)
    {
        InitPlatformContext(window);
    }

    void Context::Clear(ClearParams &)
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Context::Present()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Context::Release()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

}} // namespace GFW::OpenGL
