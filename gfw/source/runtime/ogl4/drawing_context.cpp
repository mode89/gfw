#include "common/trace.h"

#include "gfw/runtime/core/drawing_context.h"
#include "gfw/runtime/core/platform.h"
#include "gfw/runtime/core/functions.h"

#include "opengl/wglext.h"

#include <windows.h>

namespace GFW {

    class DrawingContext : public IDrawingContext
    {
    public:
        virtual RenderingContext
        CreateContext();

        virtual void
        DeleteContext(RenderingContext);

        virtual void
        MakeCurrent(RenderingContext);

        virtual RenderingContext
        GetCurrentContext();

        virtual void
        SwapBuffers();

    public:
        DrawingContext(WindowHandle);
        ~DrawingContext();

        void
        Initialize();

    private:
        IPlatformRef    mPlatform;
        HWND            mWindow;
        HDC             mDC;
        HGLRC           mDefaultContext;
    };

    DrawingContext::DrawingContext(WindowHandle window)
        : mWindow(static_cast<HWND>(window))
        , mDC(NULL)
        , mDefaultContext(NULL)
    {
        Initialize();
    }

    DrawingContext::~DrawingContext()
    {
        if (mDefaultContext != NULL)
        {
            BOOL res = wglDeleteContext(mDefaultContext);
            TRACE_ASSERT( res == TRUE ); PLAT_UNUSED( res );
        }

        if (IsWindow(mWindow) == TRUE)
        {
            BOOL res = ReleaseDC(mWindow, mDC);
            TRACE_ASSERT(res == TRUE); PLAT_UNUSED( res );
        }
    }

    void DrawingContext::Initialize()
    {
        mPlatform = IPlatform::Acquire();
        if (mPlatform.get())
        {
            TRACE_THROW("Failed to acquire OpenGL platform");
        }

        mDC = GetDC(mWindow);
        if (mDC == NULL)
        {
            TRACE_THROW("Failed to acquire drawing context of the window");
        }

        const int attribList[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     32,
            WGL_DEPTH_BITS_ARB,     24,
            WGL_STENCIL_BITS_ARB,   8,
            0,
        };

        int pixelFormat;
        UINT numFormats;

        int res = wglChoosePixelFormat(mDC, attribList, NULL, 1, &pixelFormat, &numFormats);
        if (res == FALSE)
        {
            TRACE_THROW("Failed to choose pixel format");
        }

        PIXELFORMATDESCRIPTOR pfd;
        res = SetPixelFormat(mDC, pixelFormat, &pfd);
        if (res == FALSE)
        {
            TRACE_THROW("Failed to set pixel format");
        }

        mDefaultContext = wglCreateContext(mDC);
        if (mDefaultContext == NULL)
        {
            TRACE_THROW("Failed to create the default rendering context");
        }
    }

    RenderingContext DrawingContext::CreateContext()
    {
        HGLRC hRC = wglCreateContext(mDC);
        TRACE_ASSERT(hRC != NULL);
        BOOL res = wglShareLists(mDefaultContext, hRC);
        TRACE_ASSERT(res == TRUE); PLAT_UNUSED( res );
        return hRC;
    }

    void DrawingContext::DeleteContext(RenderingContext context)
    {
        TRACE_ASSERT(context != NULL);
        BOOL res = wglDeleteContext(static_cast<HGLRC>(context));
        TRACE_ASSERT(res == TRUE); PLAT_UNUSED( res );
    }

    void DrawingContext::MakeCurrent(RenderingContext context)
    {
        BOOL res = FALSE;
        res = wglMakeCurrent(mDC, static_cast<HGLRC>(context));
        TRACE_ASSERT(res == TRUE);
    }

    RenderingContext DrawingContext::GetCurrentContext()
    {
        return wglGetCurrentContext();
    }

    void DrawingContext::SwapBuffers()
    {
        BOOL res = FALSE;
        res = ::SwapBuffers(mDC);
        TRACE_ASSERT(res == TRUE);
    }

    IDrawingContextRef CreateDrawingContext(WindowHandle window)
    {
        return std::make_shared<DrawingContext>( window );
    }

} // namespace GFW
