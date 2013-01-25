#include "gfw\graphics\opengl\platform.h"
#include "gfw\graphics\opengl\functions.h"

#if PLATFORM_WIN32
#include "gfw\platform\win\window.h"
#endif // PLATFORM_WIN32

#include "trace\trace.h"

#include <windows.h>

namespace GFW { namespace OpenGL {

    using namespace Platform;

#if PLATFORM_WIN32

    static HMODULE sLibrary = NULL;

    // Define Windows specific functions

    typedef PROC (WINAPI *  PFNWGLGETPROCADDRESS)(LPCSTR);
    typedef HGLRC (WINAPI * PFNWGLCREATECONTEXT)(HDC);
    typedef BOOL (WINAPI *  PFNWGLMAKECURRENT)(HDC, HGLRC);

    static PFNWGLGETPROCADDRESS wglGetProcAddress = NULL;
    static PFNWGLCREATECONTEXT  wglCreateContext  = NULL;
    static PFNWGLMAKECURRENT    wglMakeCurrent    = NULL;

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
        }

        return sLibrary != NULL;
    }

    uint32_t InitPlatformContext(IWindowIn window)
    {
        WindowRef wndPlat = window.StaticCast<Window>();

        HWND hWnd = wndPlat->GetWindowHandle();
        TRACE_ASSERT(hWnd != NULL);

        HDC hDC = GetDC(hWnd);
        TRACE_ASSERT(hDC != NULL);

        HGLRC hRC = wglCreateContext(hDC);
        TRACE_ASSERT(hRC != NULL);

        BOOL res = wglMakeCurrent(hDC, hRC);
        TRACE_ASSERT(res != NULL);

        return 0;
    }

#else
#error Unrecognized platform
#endif // Determine OS

}} // namespace GFW::OpenGL
