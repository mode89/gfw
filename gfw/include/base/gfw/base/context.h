#ifndef __GFW_BASE_CONTEXT_H__
#define __GFW_BASE_CONTEXT_H__

#include "gfw/base/types_fwd.h"
#include "gfw/shared/types_fwd.h"

namespace GFW {

    class IContext: public Common::ARefCounted
    {
    public:
        virtual void
        SetShader( ShaderStage stage, IShaderIn ) = 0;

        virtual void
        SetInputLayout(IInputLayoutIn) = 0;

        virtual void
        SetVertexBuffer(uint32_t slot, IBufferIn) = 0;

        virtual void
        SetIndexBuffer(IBufferIn) = 0;

        virtual void
        SetTexture(int32_t stage, uint32_t slot, ITextureIn) = 0;

        virtual void
        SetRenderTargets(uint32_t rtCount, IRenderTargetRef rt[]) = 0;

        virtual void
        BeginScene() = 0;

        virtual void
        EndScene() = 0;

        virtual void
        Clear(const ClearParams &) = 0;

        virtual void
        Draw(const DrawParams &) = 0;

        virtual void
        Draw(const DrawIndexedParams &) = 0;

        virtual void
        DrawScreenQuad() = 0;

        virtual void
        ClearState() = 0;

    public:
        virtual
        ~IContext() { }
    };
    AUTOREF_REFERENCE_DECLARATION(IContext);

} // namespace GFW

#endif // __GFW_BASE_CONTEXT_H__
