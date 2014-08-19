#include "cmn/trace.h"
#include "gfw/texture.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/texture.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/swap_chain.h"
#include "gfw/runtime/ogl4/functions.h"
#include "opengl/wglext.h"
#include <windows.h>

namespace GFW {

    SwapChain::SwapChain( const SwapChainDesc & desc, WindowHandleIn window )
        : mDesc( desc )
        , mWindow( window )
        , mWindowWidth( 0 )
        , mWindowHeight( 0 )
        , mDC()
        , mResolveFramebuffer( 0 )
        , mNativeContext()
        , mRenderTarget()
    {
        // Initialize device context

            const int kAttribList[] = {
                WGL_DRAW_TO_PBUFFER_ARB,    GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB,      GL_FALSE,
                WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
                WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB,         24,
                WGL_DEPTH_BITS_ARB,         0,
                WGL_STENCIL_BITS_ARB,       0,
                0,
            };

            if ( window )
            {
                mDC = std::shared_ptr< void >( GetDC( static_cast< HWND >( mWindow.get() ) ),
                    [ this ] ( void * dc ) {
                        BOOL res = ReleaseDC(
                            static_cast< HWND >( mWindow.get() ),
                            static_cast< HDC >( dc ) );
                        CMN_ASSERT( res == TRUE ); CMN_UNUSED( res );
                    } );
                CMN_THROW_IF( !mDC, "Failed to acquire device context of the window" );

                int pixelFormat;
                UINT numFormats;

                int res = wglChoosePixelFormatARB(
                    static_cast< HDC >( mDC.get() ),
                    kAttribList,
                    nullptr,
                    1,
                    &pixelFormat,
                    &numFormats );
                CMN_THROW_IF( res == FALSE || numFormats == 0, "Failed to choose pixel format" );

                PIXELFORMATDESCRIPTOR pfd;
                res = SetPixelFormat(
                    static_cast< HDC >( mDC.get() ),
                    pixelFormat,
                    &pfd );
                CMN_THROW_IF( res == FALSE, "Failed to set pixel format" );

                // Keep window's dimensions
                RECT windowRect;
                res = GetClientRect( static_cast< HWND >( window.get() ), &windowRect );
                CMN_ASSERT( res == TRUE );
                mWindowWidth  = windowRect.right - windowRect.left;
                mWindowHeight = windowRect.bottom - windowRect.top;
            }
            else // if no window available, create an offscreen pixel pbuffer
            {
                std::shared_ptr< void > displayDC( CreateDC( "DISPLAY", NULL, NULL, NULL ),
                    [] ( void * dc ) {
                        BOOL res = DeleteDC( static_cast< HDC >( dc ) );
                        CMN_ASSERT( res == TRUE ); CMN_UNUSED( res );
                    } );
                CMN_THROW_IF( !displayDC, "Failed to create display device context" );

                int pixelFormat = 0;
                UINT numFormats = 0;
                BOOL res = wglChoosePixelFormatARB(
                    static_cast< HDC >( displayDC.get() ),
                    kAttribList,
                    nullptr,
                    1,
                    &pixelFormat,
                    &numFormats );
                CMN_THROW_IF( res == FALSE || numFormats == 0, "Failed to choose pixel format" );

                mWindow = WindowHandleRef( wglCreatePbufferARB( static_cast< HDC >( displayDC.get() ),
                    pixelFormat, mDesc.width, mDesc.height, nullptr ),
                    [] ( void * pb ) {
                        BOOL res = wglDestroyPbufferARB( static_cast< HPBUFFERARB >( pb ) );
                        CMN_ASSERT( res != FALSE ); CMN_UNUSED( res );
                    } );
                CMN_THROW_IF( !mWindow, "Failed to create a pixel buffer" );

                mDC = std::shared_ptr< void >( wglGetPbufferDCARB( static_cast< HPBUFFERARB >( mWindow.get() ) ),
                    [ this ] ( void * dc ) {
                        int res = wglReleasePbufferDCARB(
                            static_cast< HPBUFFERARB >( mWindow.get() ),
                            static_cast< HDC >( dc ) );
                        CMN_ASSERT( res != FALSE ); CMN_UNUSED( res );
                    } );
            }

        // Create render target

            mNativeContext = CreateContext();
            MakeCurrent( mNativeContext.get() );
            {
                TextureDesc renderTargetTextureDesc;
                renderTargetTextureDesc.format      = desc.format;
                renderTargetTextureDesc.width       = desc.width;
                renderTargetTextureDesc.height      = desc.height;
                renderTargetTextureDesc.usage       = USAGE_DEFAULT;
                renderTargetTextureDesc.mipLevels   = 1;
                TextureRef renderTargetTexture( new Texture( renderTargetTextureDesc, nullptr, nullptr ),
                    [ this ] ( Texture * t ) {
                        MakeCurrent( mNativeContext.get() );
                            delete t;
                        MakeCurrent( nullptr );
                    } );

                RenderTargetDesc renderTargetDesc;
                renderTargetDesc.format = desc.format;
                mRenderTarget = RenderTargetRef( new RenderTarget( renderTargetTexture, renderTargetDesc, nullptr ),
                    [ this ] ( RenderTarget * rt ) {
                        MakeCurrent( mNativeContext.get() );
                            delete rt;
                        MakeCurrent( nullptr );
                    } );

                VGL( glGenFramebuffers, 1, &mResolveFramebuffer );
                VGL( glBindFramebuffer, GL_READ_FRAMEBUFFER, mResolveFramebuffer );
                VGL( glFramebufferTexture, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    renderTargetTexture->GetHandle(), 0 );
                VGL( glReadBuffer, GL_COLOR_ATTACHMENT0 );

                VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0 );
                VGL( glDrawBuffer, GL_LEFT );
            }
            MakeCurrent( nullptr );
    }

    SwapChain::~SwapChain()
    {
        VGL( glDeleteFramebuffers, 1, &mResolveFramebuffer );
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
        MakeCurrent( mNativeContext.get() );
            VGL( glBlitFramebuffer, 0, 0, mDesc.width, mDesc.height,
                0, 0, mWindowWidth, mWindowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR );
        MakeCurrent( nullptr );

        BOOL res = ::SwapBuffers( static_cast< HDC >( mDC.get() ) );
        CMN_ASSERT( res == TRUE ); CMN_UNUSED( res );
    }

    ConstIRenderTargetRef SwapChain::GetBuffer() const
    {
        return std::static_pointer_cast< IRenderTarget >( mRenderTarget );
    }

} // namespace GFW
