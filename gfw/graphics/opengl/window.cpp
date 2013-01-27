#include "gfw\graphics\opengl\window.h"
#include "gfw\graphics\opengl\functions_platform.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    using namespace Platform;

    OpenglWindow::OpenglWindow(IWindowIn window)
        : Window(window)
        , mWindowPlat(window.StaticCast<Window>())
    {

    }

    uint32_t OpenglWindow::Init()
    {
        HDC hDC = GetDC(mHwnd);
        TRACE_ASSERT(hDC != NULL);

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

        int res = wglChoosePixelFormat(hDC, attribList, NULL, 1, &pixelFormat, &numFormats);
        TRACE_ASSERT(res != FALSE);

        PIXELFORMATDESCRIPTOR pfd;
        res = SetPixelFormat(hDC, pixelFormat, &pfd);
        TRACE_ASSERT(res != NULL);

        mHRC = wglCreateContext(hDC);
        TRACE_ASSERT(mHRC != NULL);

        res = wglMakeCurrent(hDC, mHRC);
        TRACE_ASSERT(res != NULL);

        return 1;
    }

}} // namespace GFW::OpenGL
