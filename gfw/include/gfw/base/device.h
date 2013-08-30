#ifndef __GFW_BASE_DEVICE_H__
#define __GFW_BASE_DEVICE_H__

#include "gfw/base/types_fwd.h"

#include "gfw/base/window.h"

namespace GFW {

    class IDevice: public Common::ARefCounted
    {
    public:
        virtual IContextRef
        CreateContext() = 0;

        virtual IShaderRef
        CreateShader(int32_t stage, const void * shaderData) = 0;

        virtual IBufferRef
        CreateBuffer(const BufferDesc &, const void * initialData = 0) = 0;

        virtual ITextureRef
        CreateTexture(const TextureDesc &, const void * initialData = 0) = 0;

        virtual IRenderBufferRef
        CreateRenderBuffer(ITextureIn, const SubResIdx &) = 0;

        virtual IContextRef
        GetDefaultContext() = 0;

        virtual IRenderBufferRef
        GetDefaultColorBuffer() = 0;

        virtual void
        Present() = 0;

    public:
        virtual
        ~IDevice() { }
    };
	AUTOREF_REFERENCE_DECLARATION(IDevice);

    struct DeviceParams
    {
        WindowHandle    windowHandle;   // Platform specific window handle
        uint32_t        width;          // Back buffer width
        uint32_t        height;         // Back buffer height

        DeviceParams()
            : windowHandle(NULL)
            , width(0)
            , height(0)
        {}
    };

    IDeviceRef CreateDevice(DeviceParams &);

} // namespace GFW

#endif // __GFW_BASE_DEVICE_H__
