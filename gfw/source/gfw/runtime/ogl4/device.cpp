#include "cmn/trace.h"

#include "gfw/device_child.h"
#include "gfw/resource.h"

#include "gfw/runtime/ogl4/buffer.h"
#include "gfw/runtime/ogl4/context.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/drawing_context.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/input_layout.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/shader.h"
#include "gfw/runtime/ogl4/shader_reflect.h"
#include "gfw/runtime/ogl4/texture.h"

#define AUTO_LOCK_CONTEXT \
    /* Lock the mutex */ \
    std::lock_guard< std::mutex > __autoLock_mutex( mMutex ); \
    /* Save current context */ \
    NativeContext * __autoLock_prevContext = mSwapChain->GetCurrentContext(); \
    /* Set new context */ \
    mSwapChain->MakeCurrent( mNativeContext.get() ); \
    /* Restore context on leaving scope */ \
    std::shared_ptr< void > __autoLock_context( nullptr, \
        [ this, __autoLock_prevContext ] ( void * ) { \
            mSwapChain->MakeCurrent( __autoLock_prevContext ); \
        } )

namespace GFW {


    CMN_THREAD_LOCAL IContext * Device::mCurrentContext = NULL;

    Device::Device(const DeviceParams & params)
        : mParams(params)
        , mContextGL(NULL)
    {
        CMN_ASSERT( mParams.backBufferFormat != FORMAT_UNKNOWN );

        mDrawingContext = CreateDrawingContext(mParams.windowHandle);
        if (mDrawingContext.get())
        {
            CMN_THROW( "Failed to create a drawing context" );
        }

        mContextGL = mDrawingContext->CreateContext();
        if (mContextGL == NULL)
        {
            CMN_THROW( "Cannot create a rendering context" );
        }

        AUTO_LOCK_CONTEXT;

        InitializeSwapChain();

        mDefaultContext = std::make_shared<Context>( mDrawingContext, shared_from_this() );

        const uint8_t * extensions = glGetString(GL_EXTENSIONS);
        CMN_UNUSED( extensions );
    }

    Device::~Device()
    {
        if (mResolveFramebuffer != 0)
        {
            VGL( glDeleteFramebuffers, 1, &mResolveFramebuffer );
        }

        if (mDrawingContext && mContextGL != NULL)
        {
            mDrawingContext->MakeCurrent(NULL);
            mDrawingContext->DeleteContext(mContextGL);
        }
    }

    void Device::InitializeSwapChain()
    {
        TextureDesc defaultRenderTargetTextureDesc;
        defaultRenderTargetTextureDesc.format       = mParams.backBufferFormat;
        defaultRenderTargetTextureDesc.width        = mParams.backBufferWidth;
        defaultRenderTargetTextureDesc.height       = mParams.backBufferHeight;
        defaultRenderTargetTextureDesc.usage        = USAGE_DEFAULT;
        defaultRenderTargetTextureDesc.mipLevels    = 1;
        TextureRef defaultRenderTargetTexture = std::make_shared<Texture>(
            defaultRenderTargetTextureDesc, nullptr, shared_from_this() );

        RenderTargetDesc defaultRenderTargetDesc;
        defaultRenderTargetDesc.format = mParams.backBufferFormat;
        mDefaultRenderTarget = std::make_shared<RenderTarget>( defaultRenderTargetTexture,
            defaultRenderTargetDesc, shared_from_this() );

        VGL( glGenFramebuffers, 1, &mResolveFramebuffer );
        VGL( glBindFramebuffer, GL_READ_FRAMEBUFFER, mResolveFramebuffer );
        VGL( glFramebufferTexture, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            defaultRenderTargetTexture->GetHandle(), 0 );
        VGL( glReadBuffer, GL_COLOR_ATTACHMENT0 );

        VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0 );
        VGL( glDrawBuffer, GL_LEFT );
    }

    IContextRef Device::CreateContext()
    {
        AUTO_LOCK_CONTEXT;

        return std::make_shared<Context>( mDrawingContext, shared_from_this() );
    }

    IShaderRef Device::CreateShader( ShaderStage stage, const void * shaderBinary )
    {
        AUTO_LOCK_CONTEXT;

        return std::make_shared<Shader>( shaderBinary, stage, shared_from_this() );
    }

    IInputLayoutRef Device::CreateInputLayout( uint32_t attrCnt, VertexAttribute attrs[], ConstIShaderIn shader )
    {
        CMN_ASSERT( shader );

        AUTO_LOCK_CONTEXT;

        return std::make_shared<InputLayout>( attrCnt, attrs, shader, shared_from_this() );
    }

    IBufferRef Device::CreateBuffer( const BufferDesc & desc, const void * initialData )
    {
        AUTO_LOCK_CONTEXT;

        BufferRef buffer = std::make_shared<Buffer>( desc, shared_from_this() );

        if (buffer->Init(initialData) != 0)
        {
            return buffer;
        }

        return NULL;
    }

    ITextureRef Device::CreateTexture( const TextureDesc & desc, const void * initialData /*= 0*/ )
    {
        AUTO_LOCK_CONTEXT;

        return std::make_shared<Texture>( desc, initialData, shared_from_this() );
    }

    IRenderTargetRef Device::CreateRenderTarget( ConstITextureIn texture, const RenderTargetDesc & desc )
    {
        AUTO_LOCK_CONTEXT;

        return std::make_shared<RenderTarget>( texture, desc, shared_from_this() );
    }

    void Device::Present()
    {
        AUTO_LOCK_CONTEXT;

        VGL( glBlitFramebuffer, 0, 0, mParams.backBufferWidth, mParams.backBufferHeight,
            0, 0, mParams.backBufferWidth, mParams.backBufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST );

        mDrawingContext->SwapBuffers();
    }

    void Device::LockContext(IContextRef context)
    {
        CMN_ASSERT( context );
        CMN_ASSERT( mCurrentContext == NULL );
        mCurrentContext = context.get();
    }

    void Device::UnlockContext(IContextRef context)
    {
        CMN_ASSERT( context );
        CMN_ASSERT( mCurrentContext == context.get() );
        mCurrentContext = NULL;
    }

} // namespace GFW
