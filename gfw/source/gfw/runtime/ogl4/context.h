#ifndef __GFW_RUNTIME_CORE_CONTEXT_H__
#define __GFW_RUNTIME_CORE_CONTEXT_H__

#include "gfw/clear_params.h"
#include "gfw/context.h"
#include "gfw/draw_params.h"
#include "gfw/shader.h"
#include "gfw/shader_stage.h"
#include "gfw/vertex_attribute.h"

#include "gfw/runtime/ogl4/limits.h"
#include "gfw/runtime/ogl4/types_fwd.h"

#include <map>

namespace GFW {

    class Context: public ADeviceChild< IContext >, public std::enable_shared_from_this< Context >
    {
    public:
        virtual void
        SetShader( ShaderStage stage, ConstIShaderIn );

        virtual void
        SetInputLayout( ConstIInputLayoutIn );

        virtual void
        SetVertexBuffer( uint32_t slot, ConstIBufferIn );

        virtual void
        SetIndexBuffer( ConstIBufferIn );

        virtual void
        SetTexture( int32_t stage, uint32_t slot, ConstITextureIn );

        virtual void
        SetRenderTargets( uint32_t cpunt, ConstIRenderTargetRef rt[] );

        virtual void
        BeginScene();

        virtual void
        EndScene();

        virtual void
        Clear( const ClearParams & );

        virtual void
        Draw( const DrawParams & );

        virtual void
        Draw( const DrawIndexedParams & );

        virtual void
        DrawScreenQuad();

        virtual void
        ClearState();

    public:
        void
        FlushState();

    public:
        Context( IDrawingContextIn, DeviceIn );
        ~Context();

    private:
        void
        InitScreenQuad();

    private:
        typedef std::map < uint32_t, uint32_t > tMapProgs;

        IDrawingContextRef          mDrawingContext;

        RenderingContext            mContextGL;

        uint32_t                    mScreenQuadBuffer;

        ConstShaderRef              mShaders[ ShaderStage::COUNT ];
        uint32_t                    mProgramPipeline;

        ConstInputLayoutRef         mInputLayout;
        uint32_t                    mEnabledVertexAttributesMask;

        ConstBufferRef              mVertexBuffers[MAX_BIND_VERTEX_BUFFERS];

        ConstBufferRef              mIndexBuffer;

        int32_t                     mTextureUnits[ ShaderStage::COUNT ][ MAX_BIND_TEXTURES ];
        ConstTextureRef             mActiveTextures[MAX_BIND_TEXTURES];
        uint32_t                    mActiveTexturesDirtyMask;
        uint32_t                    mNextActiveTextureUnit;

        ConstRenderTargetRef        mRenderTargets[MAX_RENDER_TARGETS];
        uint32_t                    mRenderTargetsCount;
        uint32_t                    mDrawFramebuffer;

        bool                        mDelayedClearState;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_CONTEXT_H__
