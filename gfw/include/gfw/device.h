#ifndef __GFW_BASE_DEVICE_H__
#define __GFW_BASE_DEVICE_H__

#include "gfw/format.h"
#include "gfw/types_fwd.h"

namespace GFW {

    struct DeviceParams
    {
    };

    class IDevice
    {
    public:
        virtual IContextRef
        CreateContext() = 0;

        virtual IShaderRef
        CreateShader( ShaderStage stage, const void * shaderBinary ) = 0;

        virtual IInputLayoutRef
        CreateInputLayout( uint32_t attrCnt, VertexAttribute[], ConstIShaderIn vertexShader ) = 0;

        virtual IBufferRef
        CreateBuffer( const BufferDesc &, const void * initialData = 0 ) = 0;

        virtual ITextureRef
        CreateTexture( const TextureDesc &, const void * initialData = 0 ) = 0;

        virtual IRenderTargetRef
        CreateRenderTarget( ConstITextureIn, const RenderTargetDesc & ) = 0;

        virtual IContextRef
        GetCurrentContext() const = 0;

        // TODO rename GetImmediateContext()
        virtual IContextRef
        GetDefaultContext() const = 0;

        virtual ConstIRenderTargetRef
        GetDefaultRenderTarget() const = 0;

    public:
        virtual
        ~IDevice() { }
    };
	SHARED_PTR_TYPEDEFS( IDevice );

} // namespace GFW

#endif // __GFW_BASE_DEVICE_H__
