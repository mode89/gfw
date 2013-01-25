#include "gfw\graphics\opengl\platform.h"
#include "gfw\graphics\opengl\functions.h"

#if PLATFORM_WIN32
#include "gfw\platform\win\window.h"
#include "gfw\graphics\opengl\wglext.h"
#endif // PLATFORM_WIN32

#include "trace\trace.h"

#include <windows.h>

namespace GFW { namespace OpenGL {

    using namespace Platform;

#if PLATFORM_WIN32

    static HMODULE sLibrary = NULL;

    // Define Windows specific functions

    typedef PROC  (WINAPI *  PFNWGLGETPROCADDRESS)(LPCSTR);
    typedef HGLRC (WINAPI *  PFNWGLCREATECONTEXT)(HDC);
    typedef BOOL  (WINAPI *  PFNWGLMAKECURRENT)(HDC, HGLRC);
    typedef BOOL  (WINAPI *  PFNWGLDELETECONTEXT)(HGLRC);
    typedef BOOL  (WINAPI *  PFNWGLCHOOSEPIXELFORMAT) (HDC, const int *, const FLOAT *, UINT, int *, UINT *);

    static PFNWGLGETPROCADDRESS     wglGetProcAddress       = NULL;
    static PFNWGLCREATECONTEXT      wglCreateContext        = NULL;
    static PFNWGLMAKECURRENT        wglMakeCurrent          = NULL;
    static PFNWGLDELETECONTEXT      wglDeleteContext        = NULL;
    static PFNWGLCHOOSEPIXELFORMAT  wglChoosePixelFormat    = NULL;

    uint32_t LoadFunctions()
    {
        if (sLibrary == NULL)
        {
            sLibrary = LoadLibrary("opengl32.dll");
            TRACE_ASSERT(sLibrary != NULL);

            // Load Windows specific functions

            wglGetProcAddress = reinterpret_cast<PFNWGLGETPROCADDRESS>(GetProcAddress(sLibrary, "wglGetProcAddress"));
            TRACE_ASSERT(wglGetProcAddress != NULL);

            wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXT>(GetProcAddress(sLibrary, "wglCreateContext"));
            TRACE_ASSERT(wglCreateContext != NULL);

            wglMakeCurrent = reinterpret_cast<PFNWGLMAKECURRENT>(GetProcAddress(sLibrary, "wglMakeCurrent"));
            TRACE_ASSERT(wglMakeCurrent != NULL);

            wglDeleteContext = reinterpret_cast<PFNWGLDELETECONTEXT>(GetProcAddress(sLibrary, "wglDeleteContext"));
            TRACE_ASSERT(wglDeleteContext != NULL);

            // Create empty window

            HWND hWnd = CreateWindow("STATIC", "Window",
                WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                0, 0, 16, 16, NULL, NULL, GetModuleHandle(NULL), NULL);
            TRACE_ASSERT(hWnd != NULL);

            HDC hDC = GetDC(hWnd);
            TRACE_ASSERT(hDC != NULL);

            // Create temp rendering context

            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory(&pfd, sizeof(pfd));

            int pixelFormat = ChoosePixelFormat(hDC, &pfd);
            TRACE_ASSERT(pixelFormat != 0);

            int res = SetPixelFormat(hDC, pixelFormat, &pfd);
            TRACE_ASSERT(res != FALSE);

            HGLRC hRC = wglCreateContext(hDC);
            TRACE_ASSERT(hRC != NULL);

            res = wglMakeCurrent(hDC, hRC);
            TRACE_ASSERT(res != FALSE);

            // Load Windows specific extensions

            wglChoosePixelFormat = reinterpret_cast<PFNWGLCHOOSEPIXELFORMAT>(wglGetProcAddress("wglChoosePixelFormatARB"));
            TRACE_ASSERT(wglChoosePixelFormat != NULL);

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

        return sLibrary != NULL;
    }

    uint32_t InitPlatformContext(IWindowIn window, ContextDescPlat & desc)
    {
        WindowRef wndPlat = window.StaticCast<Window>();

        HWND hWnd = wndPlat->GetWindowHandle();
        TRACE_ASSERT(hWnd != NULL);

        HDC hDC = GetDC(hWnd);
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

        desc.hRC = wglCreateContext(hDC);
        TRACE_ASSERT(desc.hRC != NULL);

        res = wglMakeCurrent(hDC, desc.hRC);
        TRACE_ASSERT(res != NULL);

        return 0;
    }

#else
#error Unrecognized platform
#endif // Determine OS

}} // namespace GFW::OpenGL
