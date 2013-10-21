#ifndef __GFW_RUNTIME_CORE_CONTEXT_H__
#define __GFW_RUNTIME_CORE_CONTEXT_H__

#include "gfw/base/clear_params.h"
#include "gfw/base/context.h"
#include "gfw/base/draw_params.h"
#include "gfw/base/shader.h"
#include "gfw/base/vertex_attribute.h"

#include "gfw/runtime/core/limits.h"
#include "gfw/runtime/core/types_fwd.h"

#include <map>

namespace GFW {

    class Context: public IContext
    {
    public:
        virtual void
        SetShader(int32_t stage, IShaderIn);

        virtual void
        SetInputLayout(IInputLayoutIn);

        virtual void
        SetVertexBuffer(uint32_t slot, IBufferIn);

        virtual void
        SetIndexBuffer(IBufferIn);

        virtual void
        SetTexture(int32_t stage, uint32_t slot, ITextureIn);

        virtual void
        SetRenderTargets(uint32_t cpunt, IRenderTargetRef rt[]);

        virtual void
        BeginScene();

        virtual void
        EndScene();

        virtual void
        Clear(const ClearParams &);

        virtual void
        Draw(const DrawParams &);

        virtual void
        Draw(const DrawIndexedParams &);

        virtual void
        DrawScreenQuad();

        virtual void
        ClearState();

    public:
        void
        FlushState();

    public:
        Context(IDrawingContextIn, Device *);
        ~Context();

    private:
        void
        InitScreenQuad();

    private:
        typedef std::map < uint32_t, uint32_t > tMapProgs;

        Device *                    mDevice;
        IDrawingContextRef          mDrawingContext;

        RenderingContext            mContextGL;

        uint32_t                    mScreenQuadBuffer;

        ShaderRef                   mShaders[SHADER_STAGE_COUNT];
        uint32_t                    mProgramPipeline;

        InputLayoutRef              mInputLayout;
        uint32_t                    mEnabledVertexAttributesMask;

        BufferRef                   mVertexBuffers[MAX_BIND_VERTEX_BUFFERS];

        BufferRef                   mIndexBuffer;

        int32_t                     mTextureUnits[SHADER_STAGE_COUNT][MAX_BIND_TEXTURES];
        TextureRef                  mActiveTextures[MAX_BIND_TEXTURES];
        uint32_t                    mActiveTexturesDirtyMask;
        uint32_t                    mNextActiveTextureUnit;

        RenderTargetRef             mRenderTargets[MAX_RENDER_TARGETS];
        uint32_t                    mRenderTargetsCount;
        uint32_t                    mDrawFramebuffer;

        bool                        mDelayedClearState;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_CONTEXT_H__
