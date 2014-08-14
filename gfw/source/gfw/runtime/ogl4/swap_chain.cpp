#include "cmn/trace.h"
#include "gfw/texture.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/swap_chain.h"
#include "gfw/runtime/ogl4/functions.h"
#include "opengl/wglext.h"
#include <windows.h>

namespace GFW {

    SwapChain::SwapChain( const SwapChainDesc & desc, WindowHandleIn window )
        : mDesc( desc )
        , mWindow( window )
    {
        mDC = std::shared_ptr< void >( GetDC( static_cast< HWND >( mWindow.get() ) ),
            [ this ] ( void * dc ) {
                BOOL res = ReleaseDC(
                    static_cast< HWND >( mWindow.get() ),
                    static_cast< HDC >( dc ) );
                CMN_ASSERT( res == TRUE ); CMN_UNUSED( res );
            } );
        CMN_THROW_IF( !mDC, "Failed to acquire drawing context of the window" );

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

        int res = wglChoosePixelFormat(
            static_cast< HDC >( mDC.get() ),
            attribList,
            nullptr,
            1,
            &pixelFormat,
            &numFormats );
        CMN_THROW_IF( res == FALSE, "Failed to choose pixel format" );

        PIXELFORMATDESCRIPTOR pfd;
        res = SetPixelFormat(
            static_cast< HDC >( mDC.get() ),
            pixelFormat,
            &pfd );
        CMN_THROW_IF( res == FALSE, "Failed to set pixel format" );

        // Create render target

        RECT windowRect;
        res = GetClientRect( static_cast< HWND >( window.get() ), &windowRect );
        CMN_ASSERT( res == TRUE );

        TextureDesc textureDesc;
        textureDesc.width  = windowRect.right - windowRect.left;
        textureDesc.height = windowRect.bottom - windowRect.top;
        textureDesc.format = FORMAT_RGBA8_UNORM;

        RenderTargetDesc renderTargetDesc;

        mRenderTarget = std::make_shared< RenderTarget >( textureDesc, renderTargetDesc );
    }

    SwapChain::~SwapChain()
    {

    }

    NativeContextRef SwapChain::CreateContext()
    {
        NativeContextRef nativeContext( wglCreateContext( static_cast< HDC >( mDC.get() ) ),
            [] ( NativeContext * context ) {
                if ( context )
                {
                    BOOL res = wglDeleteContext( static_cast< HGLRC >( context ) );
                    CMN_ASSERT( res == TRUE); CMN_UNUSED( res );
                }
            } );
        CMN_ASSERT( nativeContext );
        return nativeContext;
    }

    void SwapChain::MakeCurrent( NativeContext * nativeContext )
    {
        BOOL res = wglMakeCurrent( static_cast< HDC >( mDC.get() ), static_cast< HGLRC >( nativeContext ) );
        CMN_ASSERT( res == TRUE ); CMN_UNUSED( res );
    }

    NativeContext * SwapChain::GetCurrentContext()
    {
        return wglGetCurrentContext();
    }

    void SwapChain::ShareLists( NativeContext * ctx1, NativeContext * ctx2 )
    {
        BOOL res = wglShareLists( static_cast< HGLRC >( ctx1 ), static_cast< HGLRC >( ctx2 ) );
        CMN_ASSERT( res == TRUE ); CMN_UNUSED( res );
    }

    void SwapChain::Present()
    {
        BOOL res = ::SwapBuffers( static_cast< HDC >( mDC.get() ) );
        CMN_ASSERT( res == TRUE ); CMN_UNUSED( res );
    }

    ConstIRenderTargetRef SwapChain::GetBuffer() const
    {
        return std::static_pointer_cast< IRenderTarget >( mRenderTarget );
    }

} // namespace GFW
