#ifndef __GFW_BASE_TYPES_FWD_H__
#define __GFW_BASE_TYPES_FWD_H__

#include "common/shared_ptr_typedefs.h"

#include <memory>

namespace GFW {

    SHARED_PTR_FORWARD_TYPEDEFS( IBlendState );
    SHARED_PTR_FORWARD_TYPEDEFS( IBuffer );
    SHARED_PTR_FORWARD_TYPEDEFS( IContext );
    SHARED_PTR_FORWARD_TYPEDEFS( IDepthStencilState );
    SHARED_PTR_FORWARD_TYPEDEFS( IDevice );
    SHARED_PTR_FORWARD_TYPEDEFS( IDeviceChild );
    SHARED_PTR_FORWARD_TYPEDEFS( IEffect );
    SHARED_PTR_FORWARD_TYPEDEFS( IFactory );
    SHARED_PTR_FORWARD_TYPEDEFS( IInputLayout );
    SHARED_PTR_FORWARD_TYPEDEFS( IMesh );
    SHARED_PTR_FORWARD_TYPEDEFS( IMeshBuilder );
    SHARED_PTR_FORWARD_TYPEDEFS( IPass );
    SHARED_PTR_FORWARD_TYPEDEFS( IRasterizerState );
    SHARED_PTR_FORWARD_TYPEDEFS( IRenderTarget );
    SHARED_PTR_FORWARD_TYPEDEFS( IResource );
    SHARED_PTR_FORWARD_TYPEDEFS( ISamplerState );
    SHARED_PTR_FORWARD_TYPEDEFS( IShader );
    SHARED_PTR_FORWARD_TYPEDEFS( IShaderBuffer );
    SHARED_PTR_FORWARD_TYPEDEFS( IShaderParameter );
    SHARED_PTR_FORWARD_TYPEDEFS( IShaderReflection );
    SHARED_PTR_FORWARD_TYPEDEFS( IShaderResource );
    SHARED_PTR_FORWARD_TYPEDEFS( IShaderVariable );
    SHARED_PTR_FORWARD_TYPEDEFS( ITechnique );
    SHARED_PTR_FORWARD_TYPEDEFS( ITexture );

    struct BufferDesc;
    struct ClearParams;
    struct DrawParams;
    struct DrawIndexedParams;
    struct EffectDesc;
    struct PassDesc;
    struct RenderTargetDesc;
    struct ResourceDesc;
    struct TechniqueDesc;
    struct TextureDesc;
    struct ShaderDesc;
    struct ShaderBufferDesc;
    struct ShaderParameterDesc;
    struct ShaderResourceDesc;
    struct ShaderVariableDesc;
    struct SubResIdx;
    struct VertexAttribute;

    typedef void * WindowHandle;

    enum ShaderStage : int32_t;

} // namespace GFW

#endif // __GFW_BASE_TYPES_FWD_H__
