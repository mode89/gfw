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

#include <functional>
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
        SetTexture( ShaderStage stage, uint32_t slot, ConstITextureIn );

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

        virtual void
        Map( IResourceIn, const SubResourceIndex &, MapType, SubResourceData & );

        virtual void
        Unmap( IResourceIn, const SubResourceIndex & );

        virtual void
        UpdateSubresource( IResourceIn, const SubResourceIndex &, const SubResourceData & );

        virtual void
        CopyResource( IResourceIn dst, IResourceIn src );

        virtual void
        Resolve( ITextureIn, const SubResourceIndex & );

    public:
        void
        FlushState();

        NativeContextRef
        GetNativeContext() const { return mNativeContext; }

    public:
        Context( NativeContextIn, DeviceIn );
        ~Context();

    private:
        void
        InitScreenQuad();

    private:

        NativeContextRef                mNativeContext;

        uint32_t                        mScreenQuadBuffer;

        // Dirty states

            struct DirtyFlags
            {
                bool renderTargets : 1;

                DirtyFlags()
                    : renderTargets( false )
                {}
            }                           mDirtyFlags;

        // Shaders

            ConstShaderRef              mShaders[ SHADER_STAGE_COUNT ];
            uint32_t                    mProgramPipeline;

        // Input buffers

            ConstInputLayoutRef         mInputLayout;
            uint32_t                    mEnabledVertexAttributesMask;

            ConstBufferRef              mVertexBuffers[ MAX_BIND_VERTEX_BUFFERS ];

            ConstBufferRef              mIndexBuffer;

        // Textures

            typedef std::map< const Texture *, uint64_t > TextureHandleMap;

            ConstTextureRef             mTextures[ SHADER_STAGE_COUNT ][ MAX_BIND_TEXTURES ];
            TextureHandleMap            mResidentTextureHandles;

        // Render targets

            ConstRenderTargetRef        mRenderTargets[MAX_RENDER_TARGETS];
            uint32_t                    mRenderTargetsCount;
            uint32_t                    mDrawFramebuffer;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_CONTEXT_H__
