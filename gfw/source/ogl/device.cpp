#include "common/trace.h"

#include "gfw/ogl/device.h"
#include "gfw/ogl/context.h"
#include "gfw/ogl/shader.h"
#include "gfw/ogl/buffer.h"
#include "gfw/graphics/opengl/functions.h"

#define AUTO_LOCK_CONTEXT   AutoLock __auto_lock_context(this, &mMutex, mNativeContext)

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
        AutoLock(Device * device, Futex * mutex, NativeContext nativeContext)
            : mDevice(device)
            , mMutex(mutex)
            , mPrevContext(NULL)
        {
            mMutex->Lock();
            mPrevContext = mDevice->GetCurrentContext();
            mDevice->MakeCurrent(nativeContext);
        }

        ~AutoLock()
        {
            mDevice->MakeCurrent(mPrevContext);
            mMutex->Unlock();
        }

    private:
        Device *        mDevice;
        Futex *         mMutex;
        NativeContext   mPrevContext;
    };

    Device::Device(const DeviceParams & params)
        : mParams(params)
        , mRenderingContext(NULL)
    {
        mDrawingContext   = CreateDrawingContext(window);
        mNativeContext    = mDrawingContext->GetRenderingContext();
        mImmediateContext = new Context(this);

        AUTO_LOCK_CONTEXT;

        const uint8_t * extensions = glGetString(GL_EXTENSIONS);
    }

    Device::~Device()
    {
        TRACE_ASSERT(mDrawingContext.IsAttached());
        TRACE_ASSERT(mRenderingContext != NULL);
        mDrawingContext->MakeCurrent(NULL);
        mDrawingContext->DeleteContext(mRenderingContext);
    }

    bool Device::Initialize()
    {
        mDrawingContext = CreateDrawingContext(mParams.windowHandle);
        if (mDrawingContext.IsNull())
        {
            TRACE_ERROR("Failed to create a drawing context");
            return false;
        }

        mRenderingContext = mDrawingContext->CreateContext();
        if (mRenderingContext == NULL)
        {
            TRACE_ERROR("Cannot create a rendering context");
            return false;
        }

        mDrawingContext->MakeCurrent(mRenderingContext);

        return true;
    }

    IContextRef Device::CreateContext()
    {
        AUTO_LOCK_CONTEXT;

        return new Context(this);
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

    bool Device::Present(bool clearState)
    {
        AUTO_LOCK_CONTEXT;

        if (clearState)
        {
            mImmediateContext->ClearState();
        }

        return mDrawingContext->SwapBuffers();
    }

    bool Device::Present()
    {
        mDrawingContext->SwapBuffers();

        return true;
    }

} // namespace GFW
