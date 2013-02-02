#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\allocator.h"

#include "common\trace.h"

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

    IShaderRef Device::CreateShader( ShaderStage, const void * shaderData )
    {
        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
    }

    IBufferRef Device::CreateBuffer( uint32_t size, const void * initialData )
    {
        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
    }

}} // namespace GFW::OpenGL
