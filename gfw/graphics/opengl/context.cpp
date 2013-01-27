#include "gfw\graphics\opengl\context.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

    Context::Context(Platform::IWindowIn window, DeviceIn d, IAllocator * a)
        : AContext(a)
        , mDevice(d)
    {
        mWindow = mDevice->GetPlatform()->CreateOpenglWindow(window);
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
