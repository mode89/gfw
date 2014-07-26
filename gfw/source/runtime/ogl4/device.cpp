#include "cmn/trace.h"

#include "gfw/base/device_child.h"
#include "gfw/base/resource.h"

#include "gfw/runtime/core/buffer.h"
#include "gfw/runtime/core/context.h"
#include "gfw/runtime/core/device.h"
#include "gfw/runtime/core/drawing_context.h"
#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/core/input_layout.h"
#include "gfw/runtime/core/render_target.h"
#include "gfw/runtime/core/shader.h"
#include "gfw/runtime/core/shader_reflect.h"
#include "gfw/runtime/core/texture.h"

#define AUTO_LOCK_CONTEXT   AutoLock __auto_lock_context(mDrawingContext.get(), mMutex, mContextGL)

namespace GFW {

    IDeviceRef CreateDevice(DeviceParams & params)
    {
        return std::static_pointer_cast<IDevice>( std::make_shared<Device>( params ) );
    }

    class AutoLock
    {
    public:
        AutoLock(IDrawingContext * dc, std::mutex & mutex, RenderingContext nativeContext)
            : mDrawingContext(dc)
            , mMutex(mutex)
            , mPrevContext(NULL)
        {
            mMutex.lock();
            mPrevContext = mDrawingContext->GetCurrentContext();
            mDrawingContext->MakeCurrent(nativeContext);
        }

        ~AutoLock()
        {
            mDrawingContext->MakeCurrent(mPrevContext);
            mMutex.unlock();
        }

        AutoLock & operator= ( const AutoLock & );

    private:
        IDrawingContext *   mDrawingContext;
        std::mutex &        mMutex;
        RenderingContext    mPrevContext;
    };

    PLAT_THREAD_LOCAL IContext * Device::mCurrentContext = NULL;

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
        PLAT_UNUSED( extensions );
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
