#include "cmn/trace.h"

#include "gfw/runtime/core/platform.h"
#include "gfw/runtime/core/functions.h"

#if defined(PLATFORM_WIN32)
    #include <windows.h>
#endif

namespace GFW {

#if defined(PLAT_WIN32)

#define F(type, func) type func = NULL;
    OPENGL_FUNCTIONS_CORE
    OPENGL_FUNCTIONS_EXT
    OPENGL_FUNCTIONS_PLAT
#undef F

    class Platform: public IPlatform
    {
    public:
        static IPlatformRef Acquire();

    protected:
        Platform();
        ~Platform();

        bool Initialize();

    private:
        static std::weak_ptr<IPlatform> mInstance;
        HMODULE                         mLibrary;

        friend class IPlatform;
    };

    std::weak_ptr<IPlatform> Platform::mInstance;

    Platform::Platform()
        : mLibrary(NULL)
    {
        Initialize();
    }

    Platform::~Platform()
    {
#define F(type, func) func = NULL;
        OPENGL_FUNCTIONS_CORE
        OPENGL_FUNCTIONS_EXT
        OPENGL_FUNCTIONS_PLAT
#undef F

        CMN_ASSERT( mLibrary != NULL );
        FreeLibrary(mLibrary);
    }

    // static
    IPlatformRef Platform::Acquire()
    {
        IPlatformRef retVal = mInstance.lock();

        if ( !retVal )
        {
            // std::make_shared() is not allowed because the destructor is protected
            IPlatform * newInstance = new Platform;
            retVal.reset( newInstance );
            mInstance = retVal;
        }

        return retVal;
    }

    bool Platform::Initialize()
    {
        bool retVal = true;

        mLibrary = LoadLibrary("opengl32.dll");
        TRACE_ASSERT_AND(mLibrary != NULL, retVal);

        // Load Windows specific functions

PLAT_WARNING_PUSH
PLAT_WARNING_DISABLE_MSVC( 4191 ) // Unsafe cast from PROC

        wglGetProcAddress = reinterpret_cast<PFNWGLGETPROCADDRESS>(GetProcAddress(mLibrary, "wglGetProcAddress"));
        TRACE_ASSERT_AND(wglGetProcAddress != NULL, retVal);

        wglCreateContext = reinterpret_cast<PFNWGLCREATECONTEXT>(GetProcAddress(mLibrary, "wglCreateContext"));
        TRACE_ASSERT_AND(wglCreateContext != NULL, retVal);

        wglMakeCurrent = reinterpret_cast<PFNWGLMAKECURRENT>(GetProcAddress(mLibrary, "wglMakeCurrent"));
        TRACE_ASSERT_AND(wglMakeCurrent != NULL, retVal);

        wglDeleteContext = reinterpret_cast<PFNWGLDELETECONTEXT>(GetProcAddress(mLibrary, "wglDeleteContext"));
        TRACE_ASSERT_AND(wglDeleteContext != NULL, retVal);

        wglGetCurrentContext = reinterpret_cast<PFNWGLGETCURRENTCONTEXT>(GetProcAddress(mLibrary, "wglGetCurrentContext"));
        TRACE_ASSERT_AND(wglGetCurrentContext != NULL, retVal);

        wglShareLists = reinterpret_cast<PFNWGLSHARELISTS>(GetProcAddress(mLibrary, "wglShareLists"));
        TRACE_ASSERT_AND(wglShareLists != NULL, retVal);

        // Create empty window

        HWND hWnd = CreateWindow("STATIC", "Window",
            WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            0, 0, 16, 16, NULL, NULL, GetModuleHandle(NULL), NULL);
        TRACE_ASSERT_AND(hWnd != NULL, retVal);

        HDC hDC = GetDC(hWnd);
        TRACE_ASSERT_AND(hDC != NULL, retVal);

        // Create temp rendering context

        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(pfd));

        int pixelFormat = ChoosePixelFormat(hDC, &pfd);
        TRACE_ASSERT_AND(pixelFormat != 0, retVal);

        int res = SetPixelFormat(hDC, pixelFormat, &pfd);
        TRACE_ASSERT_AND(res != FALSE, retVal);

        HGLRC hRC = wglCreateContext(hDC);
        TRACE_ASSERT_AND(hRC != NULL, retVal);

        res = wglMakeCurrent(hDC, hRC);
        TRACE_ASSERT_AND(res != FALSE, retVal);

        // Load Windows specific extensions

        wglChoosePixelFormat = reinterpret_cast<PFNWGLCHOOSEPIXELFORMAT>(wglGetProcAddress("wglChoosePixelFormatARB"));
        TRACE_ASSERT_AND(wglChoosePixelFormat != NULL, retVal);

        // Load core functions

#define F(type, func)   func = reinterpret_cast<type>(GetProcAddress(mLibrary, #func));
        OPENGL_FUNCTIONS_CORE
#undef F

        // Load extended functions

#define F(type, func)   func = reinterpret_cast<type>(wglGetProcAddress(#func));
        OPENGL_FUNCTIONS_EXT
#undef F

PLAT_WARNING_POP

        // Release resources

        res = wglMakeCurrent(hDC, 0);
        TRACE_ASSERT_AND( res != 0, retVal );

        res = wglDeleteContext(hRC);
        TRACE_ASSERT_AND( res != 0, retVal );

        res = ReleaseDC(hWnd, hDC);
        TRACE_ASSERT_AND( res != 0, retVal );

        res = DestroyWindow(hWnd);
        TRACE_ASSERT_AND( res != 0, retVal );

        return retVal;
    }

#endif // PLATFORM_WIN32

    // static
    IPlatformRef IPlatform::Acquire()
    {
        return Platform::Acquire();
    }

} // namespace GFW
