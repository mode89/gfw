#include "gfw\allocator.h"
#include "gfw\graphics\opengl\platform.h"
#include "gfw\graphics\opengl\functions.h"
#include "gfw\graphics\opengl\functions_platform.h"
#include "gfw\graphics\opengl\window.h"

#include "trace\trace.h"

namespace GFW { namespace OpenGL {

    using namespace Common;
    using namespace Platform;

#define F(type, func) type func = NULL;
    OPENGL_FUNCTIONS_CORE
#undef F

#if PLATFORM_WIN32

    PFNWGLGETPROCADDRESS    wglGetProcAddress       = NULL;
    PFNWGLCREATECONTEXT     wglCreateContext        = NULL;
    PFNWGLMAKECURRENT       wglMakeCurrent          = NULL;
    PFNWGLDELETECONTEXT     wglDeleteContext        = NULL;
    PFNWGLCHOOSEPIXELFORMAT wglChoosePixelFormat    = NULL;

    class PlatformWin: public ADeallocatable<IPlatform>
    {
    public:
        PlatformWin(IAllocator * a)
            : ADeallocatable(a)
            , mLibrary(NULL)
        {
            
        }

        ~PlatformWin()
        {
            if (mLibrary != NULL)
            {
                FreeLibrary(mLibrary);
            }
        }

        virtual uint32_t Init()
        {
            mLibrary = LoadLibrary("opengl32.dll");
            TRACE_ASSERT(mLibrary != NULL);

            if (mLibrary != NULL)
            {
                // Load Windows specific functions

                wglGetProcAddress = reinterpret_cast<PFNWGLGETPROCADDRESS>(GetProcAddress(mLibrary, "wglGetProcAddress"));
                TRACE_ASSERT(wglGetProcAddress != NULL);

                wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXT>(GetProcAddress(mLibrary, "wglCreateContext"));
                TRACE_ASSERT(wglCreateContext != NULL);

                wglMakeCurrent = reinterpret_cast<PFNWGLMAKECURRENT>(GetProcAddress(mLibrary, "wglMakeCurrent"));
                TRACE_ASSERT(wglMakeCurrent != NULL);

                wglDeleteContext = reinterpret_cast<PFNWGLDELETECONTEXT>(GetProcAddress(mLibrary, "wglDeleteContext"));
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

                // Load core functions

#define F(type, func)   func = reinterpret_cast<type>(GetProcAddress(mLibrary, #func));
                OPENGL_FUNCTIONS_CORE
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

            return mLibrary != NULL;
        }

        virtual IWindowRef CreateOpenglWindow(IWindowIn window)
        {
            OpenglWindowRef oglWindow = GFW_NEW(mAllocator, OpenglWindow) (window, mAllocator);
            return (oglWindow->Init() != 0) ? oglWindow.StaticCast<IWindow>() : NULL;
        }

    private:
        HMODULE         mLibrary;
    };

    IPlatformRef CreatePlatform(IAllocator * a)
    {
        return GFW_NEW(a, PlatformWin) (a);
    }

#else
#error Unrecognized platform
#endif // Determine OS

}} // namespace GFW::OpenGL
