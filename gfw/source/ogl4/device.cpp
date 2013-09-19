#include "common/trace.h"

#include "gfw/base/device_child.h"
#include "gfw/base/render_buffer.h"
#include "gfw/base/resource.h"

#include "gfw/core/buffer.h"
#include "gfw/core/context.h"
#include "gfw/core/device.h"
#include "gfw/core/drawing_context.h"
#include "gfw/core/shader.h"
#include "gfw/core/texture.h"

#include "gfw/core/functions.h"

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

    }

    Device::~Device()
    {
        TRACE_ASSERT(mDrawingContext.IsAttached());
        TRACE_ASSERT(mContextGL != NULL);
        mDrawingContext->MakeCurrent(NULL);
        mDrawingContext->DeleteContext(mContextGL);
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

        mDefaultContext = new Context(this, mDrawingContext);

        const uint8_t * extensions = glGetString(GL_EXTENSIONS);

        return true;
    }

    IContextRef Device::CreateContext()
    {
        AUTO_LOCK_CONTEXT;

        return new Context(this, mDrawingContext);
    }

    IShaderRef Device::CreateShader( ShaderStage stage, const void * shaderData )
    {
        AUTO_LOCK_CONTEXT;

        return new Shader(static_cast<const char*>(shaderData), stage, this);
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

    IRenderBufferRef Device::CreateRenderBuffer( ITextureIn, const SubResIdx & )
    {
        AUTO_LOCK_CONTEXT;

        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
    }

    void Device::Present()
    {
        AUTO_LOCK_CONTEXT;

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
