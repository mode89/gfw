#ifndef __GFW_BASE_DEVICE_H__
#define __GFW_BASE_DEVICE_H__

#include "gfw/base/format.h"
#include "gfw/base/types_fwd.h"
#include "gfw/shared/shader_stage.h"

namespace GFW {

    class IDevice: public Common::ARefCounted
    {
    public:
        virtual IContextRef
        CreateContext() = 0;

        virtual IShaderRef
        CreateShader(ShaderStage stage, const void * shaderData) = 0;

        virtual IInputLayoutRef
        CreateInputLayout(uint32_t attrCnt, VertexAttribute[], IShaderIn vertexShader) = 0;

        virtual IBufferRef
        CreateBuffer(const BufferDesc &, const void * initialData = 0) = 0;

        virtual ITextureRef
        CreateTexture(const TextureDesc &, const void * initialData = 0) = 0;

        virtual IRenderTargetRef
        CreateRenderTarget(ITextureIn, const RenderTargetDesc &) = 0;

        virtual IContextRef
        GetCurrentContext() = 0;

        virtual IContextRef
        GetDefaultContext() = 0;

        virtual IRenderTargetRef
        GetDefaultRenderTarget() = 0;

        virtual void
        Present() = 0;

    public:
        virtual
        ~IDevice() { }
    };
	AUTOREF_REFERENCE_DECLARATION(IDevice);

    struct DeviceParams
    {
        WindowHandle    windowHandle;       // Platform specific window handle
        uint32_t        backBufferWidth;
        uint32_t        backBufferHeight;
        Format          backBufferFormat;

        DeviceParams()
            : windowHandle(NULL)
            , backBufferWidth(0)
            , backBufferHeight(0)
            , backBufferFormat(FORMAT_UNKNOWN)
        {}
    };

    IDeviceRef CreateDevice(DeviceParams &);

} // namespace GFW

#endif // __GFW_BASE_DEVICE_H__
