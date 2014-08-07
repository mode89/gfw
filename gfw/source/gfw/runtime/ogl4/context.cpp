#include "cmn/trace.h"
#include "cmn/crc32.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/common/format.h"
#include "gfw/runtime/common/semantic.h"
#include "gfw/runtime/ogl4/buffer.h"
#include "gfw/runtime/ogl4/context.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/format.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/input_layout.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/shader.h"
#include "gfw/runtime/ogl4/shader_reflect.h"
#include "gfw/runtime/ogl4/shader_stage.h"
#include "gfw/runtime/ogl4/texture.h"

namespace GFW {

    using namespace Cmn;

    Context::Context( NativeContextIn nativeContext, DeviceIn d )
        : ADeviceChild( d )
        , mNativeContext( nativeContext )
        , mScreenQuadBuffer(0)
        , mProgramPipeline(0)
        , mEnabledVertexAttributesMask(0)
        , mActiveTexturesDirtyMask(0)
        , mNextActiveTextureUnit(0)
        , mRenderTargetsCount(0)
        , mDrawFramebuffer(0)
    {
        VGL( glGenProgramPipelines, 1, &mProgramPipeline );
        CMN_ASSERT( mProgramPipeline != 0 );
        VGL( glBindProgramPipeline, mProgramPipeline );

        VGL( glGenFramebuffers, 1, &mDrawFramebuffer );
        CMN_ASSERT( mDrawFramebuffer != -1 );
        VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, mDrawFramebuffer );

        InitScreenQuad();

        ClearState();
    }

    Context::~Context()
    {
        if (mProgramPipeline != 0)
        {
            VGL( glDeleteProgramPipelines, 1, &mProgramPipeline );
        }

        if (mDrawFramebuffer != 0)
        {
            VGL( glDeleteFramebuffers, 1, &mDrawFramebuffer );
        }

        if (mScreenQuadBuffer)
        {
            VGL( glDeleteBuffers, 1, &mScreenQuadBuffer );
        }
    }

    void Context::InitScreenQuad()
    {
        static const float vertices[] = {
            -1.0f, -1.0f,
            -1.0f,  1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f
        };

        VGL( glGenBuffers, 1, &mScreenQuadBuffer );
        CMN_ASSERT( mScreenQuadBuffer != 0 );
        VGL( glBindBuffer, GL_ARRAY_BUFFER, mScreenQuadBuffer );
        VGL( glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
        VGL( glBindBuffer, GL_ARRAY_BUFFER, 0 );
    }

    void Context::SetInputLayout( ConstIInputLayoutIn layout )
    {
        mInputLayout = std::static_pointer_cast<const InputLayout>( layout );
    }

    void Context::SetVertexBuffer( uint32_t slot, ConstIBufferIn buf )
    {
        CMN_ASSERT( slot < MAX_BIND_VERTEX_BUFFERS );
        CMN_ASSERT( buf );

        mVertexBuffers[slot] = std::static_pointer_cast<const Buffer>( buf );
    }

    void Context::SetShader( ShaderStage stage, ConstIShaderIn shader )
    {
        CMN_ASSERT( stage > SHADER_STAGE_UNKNOWN );
        CMN_ASSERT( stage < SHADER_STAGE_COUNT );
        CMN_ASSERT( shader );
        CMN_ASSERT( stage == shader->GetStage() );

        mShaders[stage] = std::static_pointer_cast<const Shader>( shader );
    }

    void Context::Clear(const ClearParams & cp)
    {
        uint32_t mask = 0;

        if (cp.mask & CLEAR_COLOR)
        {
            VGL( glClearColor, cp.color[0], cp.color[1], cp.color[2], cp.color[3] );
            mask |= GL_COLOR_BUFFER_BIT;
        }

		if (cp.mask & CLEAR_DEPTH)
		{
			VGL( glClearDepth, cp.depth );
			mask |= GL_DEPTH_BUFFER_BIT;
		}

        FlushState();

        VGL( glClear, mask );
    }

    void Context::DrawScreenQuad()
    {
        FlushState();

        VGL( glBindBuffer, GL_ARRAY_BUFFER, mScreenQuadBuffer );
        VGL( glVertexAttribPointer, 0, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(float), nullptr );
        VGL( glEnableVertexAttribArray, 0 );
        VGL( glBindBuffer, GL_ARRAY_BUFFER, 0 );

        VGL( glDrawArrays, GL_TRIANGLE_STRIP, 0, 4 );
    }

    void Context::Draw( const DrawParams & dp )
    {
        FlushState();

        uint32_t mode = GetOGLDrawMode(dp.primTop);
        VGL( glDrawArrays, mode, dp.vertexStart, dp.vertexCount );
    }

    void Context::Draw( const DrawIndexedParams & dp )
    {
        FlushState();

        uint32_t mode     = GetOGLDrawMode(dp.primTop);
        uint32_t type     = GetOGLType(dp.indexType);
        uint32_t typeSize = GetTypeSize(dp.indexType);

        VGL( glDrawElements, mode, dp.indexCount, type, reinterpret_cast<void*>(typeSize * dp.indexStart ));
    }

    void Context::ClearState()
    {
        // Detach shaders

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            uint32_t stageBit = GetOGLShaderStageBit(static_cast<ShaderStage>(stage));
            VGL( glUseProgramStages, mProgramPipeline, stageBit, 0 );

            mShaders[stage].reset();
        }

        // Detach buffers

        for (uint32_t i = 0; i < MAX_BIND_VERTEX_BUFFERS; ++ i)
        {
            mVertexBuffers[i].reset();
        }

        VGL( glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0 );
        mIndexBuffer.reset();

        for (uint32_t attrIndex = 0, mask = mEnabledVertexAttributesMask; mask; ++ attrIndex, mask >>= 1)
        {
            if (mask & 1)
            {
                CMN_ASSERT( attrIndex < MAX_INPUT_ATTRIBUTES );
                VGL( glDisableVertexAttribArray, attrIndex );
            }
        }
        mEnabledVertexAttributesMask = 0;

        mInputLayout.reset();

        // Detach textures

        for (uint32_t stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            for (uint32_t slot = 0; slot < MAX_BIND_TEXTURES; ++ slot)
            {
                mTextureUnits[stage][slot] = -1;
            }
        }

        for (uint32_t texUnit = 0; texUnit < MAX_BIND_TEXTURES; ++ texUnit)
        {
            if (mActiveTextures[texUnit])
            {
                VGL( glActiveTexture, GL_TEXTURE0 + texUnit );
                VGL( glBindTexture, GL_TEXTURE_2D, 0 );
                mActiveTextures[texUnit].reset();
            }
        }

        mActiveTexturesDirtyMask = 0;
        mNextActiveTextureUnit   = 0;

        VGL( glActiveTexture, GL_TEXTURE0 + MAX_BIND_TEXTURES ); // Is used as a temp texture

        // Detach render targets

        if ( mRenderTargetsCount )
        {
            for ( uint32_t i = 0; i < mRenderTargetsCount; ++ i )
            {
                mRenderTargets[ i ].reset();
            }
            mRenderTargetsCount = 0;

            mDirtyFlags.renderTargets = true;
        }
    }

    void Context::FlushState()
    {
        // Setup shaders

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            ConstShaderRef shader = mShaders[ stage ];
            uint32_t stageBit = GetOGLShaderStageBit( static_cast< ShaderStage >( stage ) );
            VGL( glUseProgramStages, mProgramPipeline, stageBit, shader ? shader->GetHandle() : 0 );
        }

        // Bind buffers

        if (mInputLayout)
        {
            for (uint32_t attrIndex = 0, mask = mInputLayout->GetEnabledAttributesMask(); mask; ++ attrIndex, mask >>= 1)
            {
                if (mask & 1)
                {
                    CMN_ASSERT( attrIndex < MAX_INPUT_ATTRIBUTES );

                    const VertexAttribute & attr = mInputLayout->GetAttribute( attrIndex );
                    CMN_ASSERT( attr.semantic != SEMANTIC_UNKNOWN );
                    CMN_ASSERT( mVertexBuffers[attr.bufSlot] );

                    VGL( glBindBuffer, GL_ARRAY_BUFFER, mVertexBuffers[ attr.bufSlot ]->GetHandle() );

                    int32_t size = GetFormatElementNumber( attr.format );
                    Type type    = GetFormatElementType( attr.format );
                    VGL( glVertexAttribPointer, attrIndex, size, GetOGLType( type ), GL_FALSE, attr.stride, reinterpret_cast< void* >( attr.offset ));
                    VGL( glEnableVertexAttribArray, attrIndex );

                    VGL( glBindBuffer, GL_ARRAY_BUFFER, 0 );
                }
            }

            for (uint32_t attrIndex = 0, mask = ( mEnabledVertexAttributesMask & !mInputLayout->GetEnabledAttributesMask() ); mask;
                ++ attrIndex, mask >>= 1)
            {
                if (mask & 1)
                {
                    VGL( glDisableVertexAttribArray, attrIndex );
                }
            }
            mEnabledVertexAttributesMask = mInputLayout->GetEnabledAttributesMask();

            if (mIndexBuffer)
            {
                VGL( glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer->GetHandle() );
            }
        }

        // Bind textures

        for (uint32_t unit = 0, mask = mActiveTexturesDirtyMask; mask; mask >>= 1, ++ unit)
        {
            if (mask & 1)
            {
                VGL( glActiveTexture, GL_TEXTURE0 + unit );
                VGL( glBindTexture, GL_TEXTURE_2D, mActiveTextures[ unit ]->GetHandle() );
            }
        }
        mActiveTexturesDirtyMask = 0;

        VGL( glActiveTexture, GL_TEXTURE0 + MAX_BIND_TEXTURES ); // Is used as a temp texture

        // Bind render targets

        if ( mDirtyFlags.renderTargets )
        {
            if ( mRenderTargetsCount > 0 )
            {
                if ( mRenderTargets[ 0 ]->IsSwapChain() )
                {
                    CMN_ASSERT( mRenderTargetsCount == 1 ); // Swap-chain cannot be mixed with custom render targets
                    VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0 );
                }
                else
                {
                    VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, mDrawFramebuffer );

                    uint32_t drawBuffers[ MAX_RENDER_TARGETS ];
                    for (uint32_t i = 0; i < mRenderTargetsCount; ++ i)
                    {
                        ConstRenderTargetRef rt = mRenderTargets[ i ];
                        CMN_ASSERT( rt->IsSwapChain() == false ); // Swap-chain cannot be mixed with custom render targets

                        ConstTextureRef rtTex   = std::static_pointer_cast< const Texture >( rt->GetTexture() );
                        VGL( glFramebufferTexture, GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rtTex->GetHandle(), rt->GetDesc().resourceIndex );
                        drawBuffers[ i ] = GL_COLOR_ATTACHMENT0 + i;
                    }
                    VGL( glDrawBuffers, mRenderTargetsCount, drawBuffers );

#ifdef CMN_DEBUG
                    int32_t status  = VGL( glCheckFramebufferStatus, GL_DRAW_FRAMEBUFFER );
                    CMN_ASSERT( status == GL_FRAMEBUFFER_COMPLETE );
#endif
                }
            }
            else
            {
                VGL( glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0 );
            }

            mDirtyFlags.renderTargets = false;
        }
    }

    void Context::SetIndexBuffer( ConstIBufferIn buffer )
    {
        mIndexBuffer = std::static_pointer_cast<const Buffer>( buffer );
    }

    void Context::SetTexture( ShaderStage stage, uint32_t slot, ConstITextureIn texture )
    {
        CMN_ASSERT( stage >= 0 );
        CMN_ASSERT( stage < SHADER_STAGE_COUNT );
        CMN_ASSERT( slot < MAX_BIND_TEXTURES );
        CMN_ASSERT( texture );

        int32_t & textureUnit = mTextureUnits[stage][slot];

        if (textureUnit == -1)
        {
            CMN_ASSERT( mActiveTextures[mNextActiveTextureUnit].get() );

            textureUnit = mNextActiveTextureUnit;
            mActiveTextures[mNextActiveTextureUnit] = std::static_pointer_cast<const Texture>( texture );
            mActiveTexturesDirtyMask |= (1 << mNextActiveTextureUnit);

            // Find a free texture unit

            uint32_t unit = (mNextActiveTextureUnit + 1) % MAX_BIND_TEXTURES;
            while (mActiveTextures[unit] && unit != mNextActiveTextureUnit)
            {
                unit = (++ unit) % MAX_BIND_TEXTURES;
            }
            mNextActiveTextureUnit = unit;
        }
        else
        {
            if (mActiveTextures[textureUnit] != texture)
            {
                // Unbind a previous texture
                if ((mActiveTexturesDirtyMask & (1 << textureUnit)) == 0)
                {
                    VGL( glActiveTexture, GL_TEXTURE0 + textureUnit );
                    VGL( glBindTexture, GL_TEXTURE_2D, 0 );
                    VGL( glActiveTexture, GL_TEXTURE0 + MAX_BIND_TEXTURES );
                }

                mActiveTextures[textureUnit] = std::static_pointer_cast<const Texture>( texture );
                mActiveTexturesDirtyMask |= (1 << textureUnit);
            }
        }
    }

    void Context::SetRenderTargets( uint32_t count, ConstIRenderTargetRef rt[] )
    {
        for ( uint32_t i = 0; i < count; ++ i )
        {
            mRenderTargets[ i ] = std::static_pointer_cast< const RenderTarget >( rt[ i ] );
        }

        for ( uint32_t i = count; i < mRenderTargetsCount; ++ i )
        {
            mRenderTargets[ i ].reset();
        }

        mRenderTargetsCount = count;

        mDirtyFlags.renderTargets = true;
    }

    void Context::BeginScene()
    {
        mDevice.lock()->LockContext( shared_from_this() );
    }

    void Context::EndScene()
    {
        ClearState();
        mDevice.lock()->UnlockContext( shared_from_this() );
    }

    void * Context::Map( IResourceIn resource, const SubResourceIndex & index, MapType mapType )
    {
        switch ( resource->GetType() )
        {
        case RESOURCE_TYPE_BUFFER:
            {
                BufferRef buffer = std::static_pointer_cast< Buffer >( resource );
                return buffer->Map( index, mapType ); 
            }
        default:
            CMN_FAIL();
        }

        return nullptr;
    }

    void Context::Unmap( IResourceIn resource, const SubResourceIndex & index )
    {
        switch ( resource->GetType() )
        {
        case RESOURCE_TYPE_BUFFER:
            {
                BufferRef buffer = std::static_pointer_cast< Buffer >( resource );
                buffer->Unmap( index );
            }
            break;
        default:
            CMN_FAIL();
        }
    }

    void Context::UpdateSubresource( IResourceIn resource, const SubResourceIndex & index, const void * data )
    {
        switch ( resource->GetType() )
        {
        case RESOURCE_TYPE_BUFFER:
            {
                BufferRef buffer = std::static_pointer_cast< Buffer >( resource );
                buffer->UpdateSubresource( index, data );
            }
            break;
        default:
            CMN_FAIL();
        }
    }

} // namespace GFW
