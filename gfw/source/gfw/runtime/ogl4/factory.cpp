#include "cmn/trace.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/factory.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/swap_chain.h"

#if defined( CMN_WIN32 )
    #include <windows.h>
#endif

namespace GFW {

#if defined( CMN_WIN32 )

#define F(type, func) type func = nullptr;
    OPENGL_FUNCTIONS_CORE
    OPENGL_FUNCTIONS_EXT
    OPENGL_FUNCTIONS_PLAT
#undef F

    static HMODULE sLibrary = nullptr;

#endif // defined( CMN_WIN32 )

    IFactoryRef CreateFactory()
    {
        return std::make_shared< Factory >();
    }

    Factory::Factory()
    {
#if defined( CMN_WIN32 )
CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4191 ) // Unsafe cast from PROC

        // Check if need to load OpenGL functions
        bool loadApi = ( sLibrary == nullptr );

        // LoadLibrary() implements reference counting, that's why it safe to call it at each construction
        sLibrary = LoadLibrary( "opengl32.dll" );
        CMN_ASSERT( sLibrary != nullptr );

        if ( loadApi )
        {
            // Create empty window

            HWND hWnd = CreateWindow( "STATIC", "Window",
                WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                0, 0, 16, 16, nullptr, nullptr, GetModuleHandle( nullptr ), nullptr );
            CMN_ASSERT( hWnd != nullptr );

            HDC hDC = GetDC( hWnd );
            CMN_ASSERT( hDC != nullptr );

            // Create temp rendering context

            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory( &pfd, sizeof( pfd ) );

            int pixelFormat = ChoosePixelFormat( hDC, &pfd );
            CMN_ASSERT( pixelFormat != 0 );

            int res = SetPixelFormat( hDC, pixelFormat, &pfd );
            CMN_ASSERT( res != FALSE );

            HGLRC hRC = wglCreateContext( hDC );
            CMN_ASSERT( hRC != nullptr );

            res = wglMakeCurrent( hDC, hRC );
            CMN_ASSERT( res != FALSE );

            // Load core functions

#define F( type, func )     func = reinterpret_cast< type >( GetProcAddress( sLibrary, #func ) );
            OPENGL_FUNCTIONS_CORE
#undef F

            // Load extended functions

#define F( type, func )     func = reinterpret_cast< type >( wglGetProcAddress( #func ) );
            OPENGL_FUNCTIONS_EXT
            OPENGL_FUNCTIONS_PLAT
#undef F

            // Release resources

            res = wglMakeCurrent( hDC, 0 );
            CMN_ASSERT( res != 0 );

            res = wglDeleteContext( hRC );
            CMN_ASSERT( res != 0 );

            res = ReleaseDC( hWnd, hDC );
            CMN_ASSERT( res != 0 );

            res = DestroyWindow( hWnd );
            CMN_ASSERT( res != 0 );
        }

CMN_WARNING_POP
#else
    #error Undefined platform
#endif // defined( CMN_WIN32 )
    }

    Factory::~Factory()
    {
#if defined( CMN_WIN32 )
        // FreeLibrary() implements reference counting, that's why it safe to call it at each destruction
        FreeLibrary( sLibrary );
#else
    #error Undefined platform
#endif
    }

    ISwapChainRef Factory::CreateSwapChain( const SwapChainDesc & desc, WindowHandleIn window )
    {
        return std::make_shared< SwapChain >( desc, window );
    }

    IDeviceRef Factory::CreateDevice( const DeviceParams & deviceParams, ISwapChainIn swapChain )
    {
        DeviceRef device = std::make_shared< Device >( deviceParams, swapChain );
        device->InitializeChildren(); // shared_from_this() cannot be called from constructor
        return device;
    }

} // namespace GFW
