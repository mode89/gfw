#include "common/trace.h"

#include "gfw/graphics/opengl/device.h"
#include "gfw/graphics/opengl/context.h"
#include "gfw/graphics/opengl/shader.h"
#include "gfw/graphics/opengl/buffer.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

    Device::Device(IPlatformIn p)
        : mPlatform(p)
    {

    }

    IContextRef Device::CreateContext(Platform::IWindowIn window)
    {
        return new Context(window, this);
    }

    IDeviceRef Device::CreateInstance()
    {
        IPlatformRef platform = CreatePlatform();

        if (platform->Init())
        {
            return new Device(platform);
        }

        return NULL;
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

    ITextureRef Device::CreateTexture( const TextureDesc &, const void * initialData /*= 0*/ )
    {
        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
    }

    IRenderBufferRef Device::CreateColorBuffer( ITextureIn, const SubResIdx & )
    {
        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
    }

}} // namespace GFW::OpenGL
