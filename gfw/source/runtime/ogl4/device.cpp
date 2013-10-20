#include "common/trace.h"

#include "gfw/base/device_child.h"
#include "gfw/base/resource.h"

#include "gfw/core/buffer.h"
#include "gfw/core/context.h"
#include "gfw/core/device.h"
#include "gfw/core/drawing_context.h"
#include "gfw/core/functions.h"
#include "gfw/core/input_layout.h"
#include "gfw/core/render_target.h"
#include "gfw/core/shader.h"
#include "gfw/core/shader_reflect.h"
#include "gfw/core/texture.h"

#define AUTO_LOCK_CONTEXT   AutoLock __auto_lock_context(mDrawingContext.GetPointer(), &mMutex, mContextGL)

namespace GFW {

    using namespace Common;

    IDeviceRef CreateDevice(DeviceParams & params)
    {
        Device * obj = new Device(params);

        if (!obj->Initialize())
        {
            TRACE_ERROR("Failed to initialize the new device");
            delete obj;
            return NULL;
        }

        return obj;
    }

    class AutoLock
    {
    public:
        AutoLock(IDrawingContext * dc, Mutex * mutex, RenderingContext nativeContext)
            : mDrawingContext(dc)
            , mMutex(mutex)
            , mPrevContext(NULL)
        {
            mMutex->Lock();
            mPrevContext = mDrawingContext->GetCurrentContext();
            mDrawingContext->MakeCurrent(nativeContext);
        }

        ~AutoLock()
        {
            mDrawingContext->MakeCurrent(mPrevContext);
            mMutex->Unlock();
        }

    private:
        IDrawingContext *   mDrawingContext;
        Mutex *             mMutex;
        RenderingContext    mPrevContext;
    };

    PLAT_THREAD_LOCAL IContext * Device::mCurrentContext = NULL;

    Device::Device(const DeviceParams & params)
        : mParams(params)
        , mContextGL(NULL)
    {
        TRACE_ASSERT(mParams.backBufferFormat != FORMAT_UNKNOWN);
    }

    Device::~Device()
    {
        if (mResolveFramebuffer != 0)
        {
            TRACE_ASSERT_GL(glDeleteFramebuffers, 1, &mResolveFramebuffer);
        }

        if (mDrawingContext.IsAttached() && mContextGL != NULL)
        {
            mDrawingContext->MakeCurrent(NULL);
            mDrawingContext->DeleteContext(mContextGL);
        }
    }

    bool Device::Initialize()
    {
        mDrawingContext = CreateDrawingContext(mParams.windowHandle);
        if (mDrawingContext.IsNull())
        {
            TRACE_ERROR("Failed to create a drawing context");
            return false;
        }

        mContextGL = mDrawingContext->CreateContext();
        if (mContextGL == NULL)
        {
            TRACE_ERROR("Cannot create a rendering context");
            return false;
        }

        AUTO_LOCK_CONTEXT;

        InitializeSwapChain();

        mDefaultContext = new Context(mDrawingContext, this);

        const uint8_t * extensions = glGetString(GL_EXTENSIONS);

        return true;
    }

    void Device::InitializeSwapChain()
    {
        TextureDesc defaultRenderTargetTextureDesc;
        defaultRenderTargetTextureDesc.format       = mParams.backBufferFormat;
        defaultRenderTargetTextureDesc.width        = mParams.backBufferWidth;
        defaultRenderTargetTextureDesc.height       = mParams.backBufferHeight;
        defaultRenderTargetTextureDesc.usage        = USAGE_DEFAULT;
        defaultRenderTargetTextureDesc.mipLevels    = 1;
        TextureRef defaultRenderTargetTexture = new Texture(defaultRenderTargetTextureDesc, NULL, this);

        RenderTargetDesc defaultRenderTargetDesc;
        defaultRenderTargetDesc.format = mParams.backBufferFormat;
        mDefaultRenderTarget = new RenderTarget(defaultRenderTargetTexture, defaultRenderTargetDesc, this);

        TRACE_ASSERT_GL(glGenFramebuffers, 1, &mResolveFramebuffer);
        TRACE_ASSERT_GL(glBindFramebuffer, GL_READ_FRAMEBUFFER, mResolveFramebuffer);
        TRACE_ASSERT_GL(glFramebufferTexture, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, defaultRenderTargetTexture->GetHandle(), 0);
        TRACE_ASSERT_GL(glReadBuffer, GL_COLOR_ATTACHMENT0);

        TRACE_ASSERT_GL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0);
        TRACE_ASSERT_GL(glDrawBuffer, GL_LEFT);
    }

    IContextRef Device::CreateContext()
    {
        AUTO_LOCK_CONTEXT;

        return new Context(mDrawingContext, this);
    }

    IShaderRef Device::CreateShader( ShaderStage stage, const void * shaderData )
    {
        AUTO_LOCK_CONTEXT;

        return new Shader(static_cast<const char*>(shaderData), stage, this);
    }

    IInputLayoutRef Device::CreateInputLayout(uint32_t attrCnt, VertexAttribute attrs[], IShaderIn shader)
    {
        TRACE_ASSERT(shader.IsAttached());

        AUTO_LOCK_CONTEXT;

        return new InputLayout(attrCnt, attrs, shader, this);
    }

    IBufferRef Device::CreateBuffer( const BufferDesc & desc, const void * initialData )
    {
        AUTO_LOCK_CONTEXT;

        BufferRef buffer = new Buffer(desc, this);

        if (buffer->Init(initialData) != 0)
        {
            return buffer.StaticCast<IBuffer>();
        }

        return NULL;
    }

    ITextureRef Device::CreateTexture( const TextureDesc & desc, const void * initialData /*= 0*/ )
    {
        AUTO_LOCK_CONTEXT;

        return new Texture(desc, initialData, this);
    }

    IRenderTargetRef Device::CreateRenderTarget(ITextureIn texture, const RenderTargetDesc & desc)
    {
        AUTO_LOCK_CONTEXT;

        return new RenderTarget(texture, desc, this);
    }

    void Device::Present()
    {
        AUTO_LOCK_CONTEXT;

        TRACE_ASSERT_GL(glBlitFramebuffer, 0, 0, mParams.backBufferWidth, mParams.backBufferHeight,
            0, 0, mParams.backBufferWidth, mParams.backBufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        mDrawingContext->SwapBuffers();
    }

    void Device::LockContext(IContextRef context)
    {
        TRACE_ASSERT(context.IsAttached());
        TRACE_ASSERT(mCurrentContext == NULL);
        mCurrentContext = context.GetPointer();
    }

    void Device::UnlockContext(IContextRef context)
    {
        TRACE_ASSERT(context.IsAttached());
        TRACE_ASSERT(mCurrentContext == context.GetPointer());
        mCurrentContext = NULL;
    }

} // namespace GFW
