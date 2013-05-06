#include "common/trace.h"

#include "gfw/graphics/opengl/device.h"
#include "gfw/graphics/opengl/context.h"
#include "gfw/graphics/opengl/shader.h"
#include "gfw/graphics/opengl/buffer.h"

#define AUTO_LOCK_CONTEXT   AutoLock __auto_lock_context(this, &mMutex, mNativeContext)

namespace GFW { namespace OpenGL {

    using namespace Common;

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

    Device::Device(WindowHandle window)
        : mNativeContext(NULL)
    {
        mDrawingContext   = CreateDrawingContext(window);
        mNativeContext    = mDrawingContext->GetRenderingContext();
        mImmediateContext = new Context(this);
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

        if (shader->Compile(static_cast<const char*>(shaderData)) != 0)
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

    IRenderBufferRef Device::CreateColorBuffer( ITextureIn, const SubResIdx & )
    {
        AUTO_LOCK_CONTEXT;

        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
    }

    bool Device::Present()
    {
        AUTO_LOCK_CONTEXT;

        return mDrawingContext->SwapBuffers();
    }

}} // namespace GFW::OpenGL
