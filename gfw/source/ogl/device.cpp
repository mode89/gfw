#include "common/trace.h"

#include "gfw/ogl/device.h"
#include "gfw/ogl/context.h"
#include "gfw/ogl/shader.h"
#include "gfw/ogl/buffer.h"

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

    Device::Device(const DeviceParams & params)
        : mParams(params)
        , mRenderingContext(NULL)
    {

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
        return new Context(this);
    }

    IShaderRef Device::CreateShader( ShaderStage stage, const void * shaderData )
    {
        ShaderRef shader = new Shader(stage);

        if (shader->Compile(static_cast<const char*>(shaderData)) != 0)
        {
            return shader.StaticCast<IShader>();
        }

        return NULL;
    }

    IBufferRef Device::CreateBuffer( const BufferDesc & desc, const void * initialData )
    {
        BufferRef buffer = new Buffer(desc);

        if (buffer->Init(initialData) != 0)
        {
            return buffer.StaticCast<IBuffer>();
        }

        return NULL;
    }

    bool Device::Present()
    {
        mDrawingContext->SwapBuffers();

        return true;
    }

} // namespace GFW
