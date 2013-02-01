#include "common\trace.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\graphics\opengl\functions.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

    Context::Context(Platform::IWindowIn window, DeviceIn d, IAllocator * a)
        : mDevice(d)
    {
        mAllocator = a;

        mWindow = mDevice->GetPlatform()->CreateOpenglWindow(window).StaticCast<OpenglWindow>();
    }

    Context::~Context()
    {

    }

    void Context::Clear(ClearParams & cp)
    {
        uint32_t mask = 0;

        if (cp.mask | CLEAR_COLOR)
        {
            TRACE_ASSERT_GL(glClearColor, cp.color[0], cp.color[1], cp.color[2], cp.color[3]);
            mask |= GL_COLOR_BUFFER_BIT;
        }

        TRACE_ASSERT_GL(glClear, mask);
    }

    void Context::Present()
    {
        mWindow->SwapBuffers();
    }

}} // namespace GFW::OpenGL
