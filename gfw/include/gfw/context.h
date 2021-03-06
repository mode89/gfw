#ifndef __GFW_BASE_CONTEXT_H__
#define __GFW_BASE_CONTEXT_H__

#include "gfw/types_fwd.h"

namespace GFW {

    class IContext
    {
    public:
        virtual void
        SetShader( ShaderStage stage, ConstIShaderIn ) = 0;

        virtual void
        SetInputLayout( ConstIInputLayoutIn ) = 0;

        virtual void
        SetVertexBuffer( uint32_t slot, ConstIBufferIn ) = 0;

        virtual void
        SetIndexBuffer( ConstIBufferIn ) = 0;

        virtual void
        SetConstantBuffer( ShaderStage stage, uint32_t slot, ConstIBufferIn ) = 0;

        virtual void
        SetTexture( ShaderStage stage, uint32_t slot, ConstITextureIn ) = 0;

        virtual void
        SetRenderTargets( uint32_t rtCount, ConstIRenderTargetRef rt[] ) = 0;

        virtual void
        BeginScene() = 0;

        virtual void
        EndScene() = 0;

        virtual void
        Clear( const ClearParams & ) = 0;

        virtual void
        Draw( const DrawParams & ) = 0;

        virtual void
        Draw( const DrawIndexedParams & ) = 0;

        virtual void
        DrawScreenQuad() = 0;

        virtual void
        ClearState() = 0;

        virtual void
        Map( IResourceIn, const SubResourceIndex &, MapType, SubResourceData & ) = 0;

        virtual void
        Unmap( IResourceIn, const SubResourceIndex & ) = 0;

        virtual void
        UpdateSubresource( IResourceIn, const SubResourceIndex &, const SubResourceData & ) = 0;

        virtual void
        CopyResource( IResourceIn dst, IResourceIn src ) = 0;

        virtual void
        Resolve( ITextureIn, const SubResourceIndex & ) = 0;

    public:
        virtual
        ~IContext() { }
    };
    SHARED_PTR_TYPEDEFS( IContext );

} // namespace GFW

#endif // __GFW_BASE_CONTEXT_H__
