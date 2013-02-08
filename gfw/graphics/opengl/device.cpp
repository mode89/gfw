#include "common/trace.h"

#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\graphics\opengl\shader.h"
#include "gfw\graphics\opengl\buffer.h"
#include "gfw\allocator.h"

namespace GFW { namespace OpenGL {

    using namespace Common;

    Device::Device(IPlatformIn p, IAllocator * a)
        : mPlatform(p)
    {
        mAllocator = a;
    }

    IContextRef Device::CreateContext(Platform::IWindowIn window)
    {
        return GFW_NEW(mAllocator, Context) (window, this, mAllocator);
    }

    IDeviceRef Device::CreateInstance(IAllocator * a)
    {
        IPlatformRef platform = CreatePlatform(a);

        if (platform->Init())
        {
            return GFW_NEW(a, Device) (platform, a);
        }

        return NULL;
    }

    IShaderRef Device::CreateShader( ShaderStage stage, const void * shaderData )
    {
        ShaderRef shader = GFW_NEW(mAllocator, Shader) (stage, mAllocator);

        if (shader->Compile(static_cast<const char*>(shaderData)) != 0)
        {
            return shader.StaticCast<IShader>();
        }

        return NULL;
    }

    IBufferRef Device::CreateBuffer( const BufferDesc & desc, const void * initialData )
    {
        BufferRef buffer = GFW_NEW(mAllocator, Buffer) (desc, mAllocator);

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
