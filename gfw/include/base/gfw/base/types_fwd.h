#ifndef __GFW_BASE_TYPES_FWD_H__
#define __GFW_BASE_TYPES_FWD_H__

#include "common/autoref.h"

namespace GFW {

    AUTOREF_FORWARD_DECLARATION( IBlendState );
    AUTOREF_FORWARD_DECLARATION( IBuffer );
    AUTOREF_FORWARD_DECLARATION( IContext );
    AUTOREF_FORWARD_DECLARATION( IDepthStencilState );
    AUTOREF_FORWARD_DECLARATION( IDevice );
    AUTOREF_FORWARD_DECLARATION( IDeviceChild );
    AUTOREF_FORWARD_DECLARATION( IEffect );
    AUTOREF_FORWARD_DECLARATION( IFactory );
    AUTOREF_FORWARD_DECLARATION( IInputLayout );
    AUTOREF_FORWARD_DECLARATION( IMesh );
    AUTOREF_FORWARD_DECLARATION( IMeshBuilder );
    AUTOREF_FORWARD_DECLARATION( IPass );
    AUTOREF_FORWARD_DECLARATION( IRasterizerState );
    AUTOREF_FORWARD_DECLARATION( IRenderTarget );
    AUTOREF_FORWARD_DECLARATION( IResource );
    AUTOREF_FORWARD_DECLARATION( ISamplerState );
    AUTOREF_FORWARD_DECLARATION( IShader );
    AUTOREF_FORWARD_DECLARATION( IShaderBuffer );
    AUTOREF_FORWARD_DECLARATION( IShaderParameter );
    AUTOREF_FORWARD_DECLARATION( IShaderReflection );
    AUTOREF_FORWARD_DECLARATION( IShaderResource );
    AUTOREF_FORWARD_DECLARATION( IShaderVariable );
    AUTOREF_FORWARD_DECLARATION( ITechnique );
    AUTOREF_FORWARD_DECLARATION( ITexture );

    struct BufferDesc;
    struct ClearParams;
    struct DrawParams;
    struct DrawIndexedParams;
    struct RenderTargetDesc;
    struct ResourceDesc;
    struct TextureDesc;
    struct ShaderDesc;
    struct ShaderBufferDesc;
    struct ShaderParameterDesc;
    struct ShaderResourceDesc;
    struct ShaderVariableDesc;
    struct SubResIdx;
    struct VertexAttribute;

    typedef void * WindowHandle;

} // namespace GFW

#endif // __GFW_BASE_TYPES_FWD_H__
