#include "common/trace.h"

#include "gfw/ogl/device.h"
#include "gfw/ogl/context.h"
#include "gfw/ogl/shader.h"
#include "gfw/ogl/buffer.h"
#include "gfw/ogl/functions.h"

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
        AutoLock(IDrawingContext * dc, Futex * mutex, RenderingContext nativeContext)
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
        Futex *             mMutex;
        RenderingContext    mPrevContext;
    };

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

        mDefaultContext = new Context(this, mDrawingContext);

        AUTO_LOCK_CONTEXT;

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

        ShaderRef shader = new Shader(stage);

        if (shader->Compile(static_cast<const char*>(shaderData)))
        {
            return shader.StaticCast<IShader>();
        }

        return NULL;
    }

    IBufferRef Device::CreateBuffer( const BufferDesc & desc, const void * initialData )
    {
        AUTO_LOCK_CONTEXT;

        BufferRef buffer = new Buffer(desc);

        if (buffer->Init(initialData) != 0)
        {
            return buffer.StaticCast<IBuffer>();
        }

        return NULL;
    }

    ITextureRef Device::CreateTexture( const TextureDesc &, const void * initialData /*= 0*/ )
    {
        AUTO_LOCK_CONTEXT;

        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
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

} // namespace GFW
