#include "gfw\graphics\opengl\window.h"
#include "gfw\graphics\opengl\functions_platform.h"

#include "common\trace.h"

namespace GFW { namespace OpenGL {

    using namespace Common;
    using namespace Platform;

    OpenglWindow::OpenglWindow(IWindowIn window, IAllocator * a)
        : mWindowPlat(window.StaticCast<Window>())
    {
        mAllocator = a;

        mHWND = mWindowPlat->GetWindowHandle();
        TRACE_ASSERT(mHWND != NULL);

        mHDC  = GetDC(mHWND);
        TRACE_ASSERT(mHDC != NULL);
    }

    OpenglWindow::~OpenglWindow()
    {
        int res = 0;

        res = wglMakeCurrent(mHDC, NULL);
        TRACE_ASSERT(res != NULL);

        mHDC = NULL;

        res = wglDeleteContext(mHRC);
        TRACE_ASSERT(res != NULL);

        mHRC = NULL;
    }

    void OpenglWindow::Tick()
    {
        mWindowPlat->Tick();
    }

    uint32_t OpenglWindow::Init()
    {
        const int attribList[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     32,
            WGL_DEPTH_BITS_ARB,     24,
            WGL_STENCIL_BITS_ARB,   8,
            0,
        };

        int pixelFormat;
        UINT numFormats;

        int res = wglChoosePixelFormat(mHDC, attribList, NULL, 1, &pixelFormat, &numFormats);
        TRACE_ASSERT(res != FALSE);

        PIXELFORMATDESCRIPTOR pfd;
        res = SetPixelFormat(mHDC, pixelFormat, &pfd);
        TRACE_ASSERT(res != NULL);

        mHRC = wglCreateContext(mHDC);
        TRACE_ASSERT(mHRC != NULL);

        res = wglMakeCurrent(mHDC, mHRC);
        TRACE_ASSERT(res != NULL);

        return 1;
    }

    void OpenglWindow::SwapBuffers()
    {
        ::SwapBuffers(mHDC);
    }

}} // namespace GFW::OpenGL
