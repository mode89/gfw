#include "common/trace.h"

#include "gfw/graphics/opengl/drawing_context.h"
#include "gfw/graphics/opengl/functions.h"
#include "gfw/graphics/opengl/functions_platform.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

#define F(type, func) type func = NULL;
    OPENGL_FUNCTIONS_CORE
    OPENGL_FUNCTIONS_EXT
#undef F

#if PLATFORM_WIN32

    PFNWGLGETPROCADDRESS    wglGetProcAddress       = NULL;
    PFNWGLCREATECONTEXT     wglCreateContext        = NULL;
    PFNWGLMAKECURRENT       wglMakeCurrent          = NULL;
    PFNWGLGETCURRENTCONTEXT wglGetCurrentContext    = NULL;
    PFNWGLDELETECONTEXT     wglDeleteContext        = NULL;
    PFNWGLCHOOSEPIXELFORMAT wglChoosePixelFormat    = NULL;
    PFNWGLSHARELISTS        wglShareLists           = NULL;

    static HMODULE sLibrary = NULL;

#define ASSERT(cond, msg) \
    if (!(cond)) \
    { \
        if (sLibrary != NULL) \
        { \
            FreeLibrary(sLibrary); \
            sLibrary = NULL; \
        } \
        TRACE_ERROR(msg); \
        return false; \
    }

    bool InitializeInterface()
    {
        if (sLibrary == NULL)
        {
            sLibrary = LoadLibrary("opengl32.dll");
            ASSERT(sLibrary != NULL, "Failed to load OpenGL library.");

            // Load Windows specific functions

            wglGetProcAddress = reinterpret_cast<PFNWGLGETPROCADDRESS>(GetProcAddress(sLibrary, "wglGetProcAddress"));
            ASSERT(wglGetProcAddress != NULL, "Failed to load wglGetProcAddress().");

            wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXT>(GetProcAddress(sLibrary, "wglCreateContext"));
            ASSERT(wglCreateContext != NULL, "Failed to load wglCreateContext().");

            wglMakeCurrent = reinterpret_cast<PFNWGLMAKECURRENT>(GetProcAddress(sLibrary, "wglMakeCurrent"));
            ASSERT(wglMakeCurrent != NULL, "Failed to load wglMakeCurrent().");

            wglGetCurrentContext = reinterpret_cast<PFNWGLGETCURRENTCONTEXT>(GetProcAddress(sLibrary, "wglGetCurrentContext"));
            ASSERT(wglGetCurrentContext != NULL, "Failed to load wglGetCurrentContext().");

            wglDeleteContext = reinterpret_cast<PFNWGLDELETECONTEXT>(GetProcAddress(sLibrary, "wglDeleteContext"));
            ASSERT(wglDeleteContext != NULL, "Failed to load wglDeleteContext().");

            wglShareLists = reinterpret_cast<PFNWGLSHARELISTS>(GetProcAddress(sLibrary, "wglShareLists"));
            ASSERT(wglShareLists != NULL, "Failed to load wglShareLists");

            // Create empty window

            HWND hWnd = CreateWindow("STATIC", "Window",
                WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                0, 0, 16, 16, NULL, NULL, GetModuleHandle(NULL), NULL);
            ASSERT(hWnd != NULL, "Failed to create a temporary window.");

            HDC hDC = GetDC(hWnd);
            ASSERT(hDC != NULL, "Failed to retrieve a device context of he temporary window.");

            // Create temp rendering context

            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory(&pfd, sizeof(pfd));

            int pixelFormat = ChoosePixelFormat(hDC, &pfd);
            ASSERT(pixelFormat != 0, "Failed to choose a pixel format.");

            int res = SetPixelFormat(hDC, pixelFormat, &pfd);
            ASSERT(res != FALSE, "Failed to set a pixel format.");

            HGLRC hRC = wglCreateContext(hDC);
            ASSERT(hRC != NULL, "Failed to create a rendering context.");

            res = wglMakeCurrent(hDC, hRC);
            ASSERT(res != FALSE, "Failed to setup current rendering context.");

            // Load Windows specific extensions

            wglChoosePixelFormat = reinterpret_cast<PFNWGLCHOOSEPIXELFORMAT>(wglGetProcAddress("wglChoosePixelFormatARB"));
            ASSERT(wglChoosePixelFormat != NULL, "Failed to load wglChoosePixelFormat().");

            // Load core functions

#define F(type, func)   func = reinterpret_cast<type>(GetProcAddress(sLibrary, #func));
            OPENGL_FUNCTIONS_CORE
#undef F

            // Load extended functions
#define F(type, func)   func = reinterpret_cast<type>(wglGetProcAddress(#func));
            OPENGL_FUNCTIONS_EXT
#undef F

            // Release resources

            res = wglMakeCurrent(hDC, 0);
            TRACE_ASSERT(res != NULL);

            res = wglDeleteContext(hRC);
            TRACE_ASSERT(res != NULL);

            res = ReleaseDC(hWnd, hDC);
            TRACE_ASSERT(res != NULL);

            res = DestroyWindow(hWnd);
            TRACE_ASSERT(res != NULL);
        }

        return true;
    }

#undef ASSERT

    class DrawingContext: public IDrawingContext
    {
    public:
        DrawingContext(WindowHandle window)
            : mHwnd(static_cast<HWND>(window))
            , mHDC(NULL)
            , mHRC(NULL)
        {
            TRACE_ASSERT(mHwnd != NULL);

            mHDC = GetDC(mHwnd);
            TRACE_ASSERT(mHDC != NULL);

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

            int res = wglChoosePixelFormat(mHDC, attribList, NULL, 1, &pixelFormat, &numFormats);
            TRACE_ASSERT(res != FALSE);

            PIXELFORMATDESCRIPTOR pfd;
            res = SetPixelFormat(mHDC, pixelFormat, &pfd);
            TRACE_ASSERT(res != NULL);

            mHRC = wglCreateContext(mHDC);
            TRACE_ASSERT(mHRC != NULL);
        }

        ~DrawingContext()
        {
            BOOL res = FALSE;

            TRACE_ASSERT(mHRC != NULL);
            res = wglDeleteContext(mHRC);
            TRACE_ASSERT(res != FALSE);

            mHRC = NULL;

            TRACE_ASSERT(mHDC != NULL);
            res = ReleaseDC(mHwnd, mHDC);
            TRACE_ASSERT(res != FALSE);

            mHDC = NULL;
        }

        virtual NativeContext CreateContext()
        {
            HGLRC newContext = wglCreateContext(mHDC);
            TRACE_ASSERT(newContext != NULL);

            BOOL res = wglShareLists(newContext, mHRC);
            TRACE_ASSERT(res != FALSE);

            return newContext;
        }

        virtual void DeleteContext(NativeContext nativeContext)
        {
            BOOL res = wglDeleteContext(static_cast<HGLRC>(nativeContext));
            TRACE_ASSERT(res != FALSE);
        }

        virtual void MakeCurrent(NativeContext context)
        {
            BOOL res = wglMakeCurrent(mHDC, static_cast<HGLRC>(context));
            TRACE_ASSERT(res != NULL);
        }

        virtual NativeContext GetCurrentContext()
        {
            return wglGetCurrentContext();
        }

        virtual bool SwapBuffers()
        {
            return ::SwapBuffers(mHDC) != FALSE;
        }

        virtual NativeContext GetRenderingContext()
        {
            return mHRC;
        }

    private:
        HWND    mHwnd;
        HDC     mHDC;
        HGLRC   mHRC;
    };

#else
#error Unrecognized platform
#endif // Determine OS

    IDrawingContextRef CreateDrawingContext(WindowHandle window)
    {
        if (!InitializeInterface())
        {
            TRACE_ERROR("Cannot initialize OpenGL interface.");
            return NULL;
        }

        return new DrawingContext(window);
    }

}} // namespace GFW::OpenGL
