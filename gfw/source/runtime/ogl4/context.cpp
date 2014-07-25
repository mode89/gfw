#include "cmn/trace.h"
#include "cmn/crc32.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/common/format.h"
#include "gfw/runtime/common/semantic.h"
#include "gfw/runtime/core/buffer.h"
#include "gfw/runtime/core/context.h"
#include "gfw/runtime/core/device.h"
#include "gfw/runtime/core/drawing_context.h"
#include "gfw/runtime/core/format.h"
#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/core/input_layout.h"
#include "gfw/runtime/core/render_target.h"
#include "gfw/runtime/core/shader.h"
#include "gfw/runtime/core/shader_reflect.h"
#include "gfw/runtime/core/shader_stage.h"
#include "gfw/runtime/core/texture.h"

namespace GFW {

    using namespace Common;

    Context::Context( IDrawingContextIn dc, DeviceIn d )
        : ADeviceChild( d )
        , mDrawingContext(dc)
        , mContextGL(NULL)
        , mScreenQuadBuffer(0)
        , mProgramPipeline(0)
        , mEnabledVertexAttributesMask(0)
        , mActiveTexturesDirtyMask(0)
        , mNextActiveTextureUnit(0)
        , mRenderTargetsCount(0)
        , mDrawFramebuffer(0)
        , mDelayedClearState(false)
    {
        mContextGL = mDrawingContext->CreateContext();

        // Create context specific objects

        RenderingContext currentContext = mDrawingContext->GetCurrentContext();
        mDrawingContext->MakeCurrent(mContextGL);
        {
            TRACE_ASSERT_GL(glGenProgramPipelines, 1, &mProgramPipeline);
            TRACE_ASSERT(mProgramPipeline != 0);
            TRACE_ASSERT_GL(glBindProgramPipeline, mProgramPipeline);

            TRACE_ASSERT_GL(glGenFramebuffers, 1, &mDrawFramebuffer);
            TRACE_ASSERT(mDrawFramebuffer != -1);
            TRACE_ASSERT_GL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, mDrawFramebuffer);

            InitScreenQuad();
        }
        mDrawingContext->MakeCurrent(currentContext);

        ClearState();
    }

    Context::~Context()
    {
        // Delete context specific objects

        RenderingContext currentContext = mDrawingContext->GetCurrentContext();
        mDrawingContext->MakeCurrent(mContextGL);
        {
            if (mProgramPipeline != 0)
            {
                TRACE_ASSERT_GL(glDeleteProgramPipelines, 1, &mProgramPipeline);
            }

            if (mDrawFramebuffer != 0)
            {
                TRACE_ASSERT_GL(glDeleteFramebuffers, 1, &mDrawFramebuffer);
            }

            if (mScreenQuadBuffer)
            {
                TRACE_ASSERT_GL(glDeleteBuffers, 1, &mScreenQuadBuffer);
            }
        }
        mDrawingContext->MakeCurrent(currentContext);

        if (mContextGL != NULL)
        {
            mDrawingContext->DeleteContext(mContextGL);
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

        TRACE_ASSERT_GL(glGenBuffers, 1, &mScreenQuadBuffer);
        TRACE_ASSERT(mScreenQuadBuffer != 0);
        TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, mScreenQuadBuffer);
        TRACE_ASSERT_GL(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, 0);
    }

    void Context::SetInputLayout( ConstIInputLayoutIn layout )
    {
        mInputLayout = std::static_pointer_cast<const InputLayout>( layout );
    }

    void Context::SetVertexBuffer( uint32_t slot, ConstIBufferIn buf )
    {
        TRACE_ASSERT(slot < MAX_BIND_VERTEX_BUFFERS);
        TRACE_ASSERT(buf);

        mVertexBuffers[slot] = std::static_pointer_cast<const Buffer>( buf );
    }

    void Context::SetShader( ShaderStage stage, ConstIShaderIn shader )
    {
        TRACE_ASSERT( stage > ShaderStage::UNKNOWN );
        TRACE_ASSERT( stage < ShaderStage::COUNT );
        TRACE_ASSERT( shader );
        TRACE_ASSERT( stage == shader->GetStage() );

        mShaders[stage] = std::static_pointer_cast<const Shader>( shader );
    }

    void Context::Clear(const ClearParams & cp)
    {
        uint32_t mask = 0;

        if (cp.mask & CLEAR_COLOR)
        {
            TRACE_ASSERT_GL(glClearColor, cp.color[0], cp.color[1], cp.color[2], cp.color[3]);
            mask |= GL_COLOR_BUFFER_BIT;
        }

		if (cp.mask & CLEAR_DEPTH)
		{
			TRACE_ASSERT_GL(glClearDepth, cp.depth);
			mask |= GL_DEPTH_BUFFER_BIT;
		}

        FlushState();

        TRACE_ASSERT_GL(glClear, mask);
    }

    void Context::DrawScreenQuad()
    {
        FlushState();

        TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, mScreenQuadBuffer);
        TRACE_ASSERT_GL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(float), NULL);
        TRACE_ASSERT_GL(glEnableVertexAttribArray, 0);
        TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, 0);

        TRACE_ASSERT_GL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
    }

    void Context::Draw( const DrawParams & dp )
    {
        FlushState();

        uint32_t mode = GetOGLDrawMode(dp.primTop);
        TRACE_ASSERT_GL(glDrawArrays, mode, dp.vertexStart, dp.vertexCount);
    }

    void Context::Draw( const DrawIndexedParams & dp )
    {
        FlushState();

        uint32_t mode     = GetOGLDrawMode(dp.primTop);
        uint32_t type     = GetOGLType(dp.indexType);
        uint32_t typeSize = GetTypeSize(dp.indexType);

        TRACE_ASSERT_GL(glDrawElements, mode, dp.indexCount, type, reinterpret_cast<void*>(typeSize * dp.indexStart));
    }

    void Context::ClearState()
    {
        if ( mDevice.lock()->GetCurrentContext().get() != this)
        {
            mDelayedClearState = true;
            return;
        }

        // Detach shaders

        for (int stage = 0; stage < ShaderStage::COUNT; ++ stage)
        {
            uint32_t stageBit = GetOGLShaderStageBit(static_cast<ShaderStage>(stage));
            TRACE_ASSERT_GL(glUseProgramStages, mProgramPipeline, stageBit, 0);

            mShaders[stage].reset();
        }

        // Detach buffers

        for (uint32_t i = 0; i < MAX_BIND_VERTEX_BUFFERS; ++ i)
        {
            mVertexBuffers[i].reset();
        }

        TRACE_ASSERT_GL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
        mIndexBuffer.reset();

        for (uint32_t attrIndex = 0, mask = mEnabledVertexAttributesMask; mask; ++ attrIndex, mask >>= 1)
        {
            if (mask & 1)
            {
                TRACE_ASSERT(attrIndex < MAX_INPUT_ATTRIBUTES);
                TRACE_ASSERT_GL(glDisableVertexAttribArray, attrIndex);
            }
        }
        mEnabledVertexAttributesMask = 0;

        mInputLayout.reset();

        // Detach textures

        for (uint32_t stage = 0; stage < ShaderStage::COUNT; ++ stage)
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
                TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + texUnit);
                TRACE_ASSERT_GL(glBindTexture, GL_TEXTURE_2D, 0);
                mActiveTextures[texUnit].reset();
            }
        }

        mActiveTexturesDirtyMask = 0;
        mNextActiveTextureUnit   = 0;

        TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + MAX_BIND_TEXTURES); // Is used as a temp texture

        // Detach render targets

        for (uint32_t i = 0; i < mRenderTargetsCount; ++ i)
        {
            TRACE_ASSERT_GL(glFramebufferTexture, GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 0, 0);
            mRenderTargets[i].reset();
        }
        TRACE_ASSERT_GL(glDrawBuffer, GL_NONE);
        mRenderTargetsCount = 0;
    }

    void Context::FlushState()
    {
        // Setup shaders

        for (int stage = 0; stage < ShaderStage::COUNT; ++ stage)
        {
            ConstShaderRef shader = mShaders[stage];
            uint32_t stageBit = GetOGLShaderStageBit(static_cast<ShaderStage>(stage));
            TRACE_ASSERT_GL(glUseProgramStages, mProgramPipeline, stageBit, shader ? shader->GetHandle() : 0);
        }

        // Bind buffers

        if (mInputLayout)
        {
            for (uint32_t attrIndex = 0, mask = mInputLayout->GetEnabledAttributesMask(); mask; ++ attrIndex, mask >>= 1)
            {
                if (mask & 1)
                {
                    TRACE_ASSERT(attrIndex < MAX_INPUT_ATTRIBUTES);

                    const VertexAttribute & attr = mInputLayout->GetAttribute(attrIndex);
                    TRACE_ASSERT(attr.semantic != SEMANTIC_UNKNOWN);
                    TRACE_ASSERT(mVertexBuffers[attr.bufSlot]);

                    TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, mVertexBuffers[attr.bufSlot]->GetHandle());

                    int32_t size = GetFormatElementNumber(attr.format);
                    Type type    = GetFormatElementType(attr.format);
                    TRACE_ASSERT_GL(glVertexAttribPointer, attrIndex, size, GetOGLType(type), GL_FALSE, attr.stride, reinterpret_cast<void*>(attr.offset));
                    TRACE_ASSERT_GL(glEnableVertexAttribArray, attrIndex);

                    TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, 0);
                }
            }

            for (uint32_t attrIndex = 0, mask = (mEnabledVertexAttributesMask & !mInputLayout->GetEnabledAttributesMask()); mask;
                ++ attrIndex, mask >>= 1)
            {
                if (mask & 1)
                {
                    TRACE_ASSERT_GL(glDisableVertexAttribArray, attrIndex);
                }
            }
            mEnabledVertexAttributesMask = mInputLayout->GetEnabledAttributesMask();

            if (mIndexBuffer)
            {
                TRACE_ASSERT_GL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer->GetHandle());
            }
        }

        // Bind textures

        for (uint32_t unit = 0, mask = mActiveTexturesDirtyMask; mask; mask >>= 1, ++ unit)
        {
            if (mask & 1)
            {
                TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + unit);
                TRACE_ASSERT_GL(glBindTexture, GL_TEXTURE_2D, mActiveTextures[unit]->GetHandle());
            }
        }
        mActiveTexturesDirtyMask = 0;

        TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + MAX_BIND_TEXTURES); // Is used as a temp texture

        // Bind render targets

        uint32_t drawBuffers[MAX_RENDER_TARGETS];
        for (uint32_t i = 0; i < mRenderTargetsCount; ++ i)
        {
            ConstRenderTargetRef rt = mRenderTargets[i];
            ConstTextureRef rtTex   = std::static_pointer_cast<const Texture>( rt->GetTexture() );
            TRACE_ASSERT_GL(glFramebufferTexture, GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rtTex->GetHandle(), rt->GetDesc().resourceIndex);
            drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        TRACE_ASSERT_GL(glDrawBuffers, mRenderTargetsCount, drawBuffers);

#ifdef TRACE_ASSERT_ENABLED
        int32_t status  = TRACE_ASSERT_GL(glCheckFramebufferStatus, GL_DRAW_FRAMEBUFFER);
        TRACE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);
#endif
    }

    void Context::SetIndexBuffer( ConstIBufferIn buffer )
    {
        mIndexBuffer = std::static_pointer_cast<const Buffer>( buffer );
    }

    void Context::SetTexture( int32_t stage, uint32_t slot, ConstITextureIn texture )
    {
        TRACE_ASSERT(stage >= 0);
        TRACE_ASSERT(stage < ShaderStage::COUNT);
        TRACE_ASSERT(slot < MAX_BIND_TEXTURES);
        TRACE_ASSERT(texture);

        int32_t & textureUnit = mTextureUnits[stage][slot];

        if (textureUnit == -1)
        {
            TRACE_ASSERT(mActiveTextures[mNextActiveTextureUnit].get());

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
                    TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + textureUnit);
                    TRACE_ASSERT_GL(glBindTexture, GL_TEXTURE_2D, 0);
                    TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + MAX_BIND_TEXTURES);
                }

                mActiveTextures[textureUnit] = std::static_pointer_cast<const Texture>( texture );
                mActiveTexturesDirtyMask |= (1 << textureUnit);
            }
        }
    }

    void Context::SetRenderTargets( uint32_t count, ConstIRenderTargetRef rt[] )
    {
        for (uint32_t i = 0; i < count; ++ i)
        {
            mRenderTargets[i] = std::static_pointer_cast<const RenderTarget>( rt[i] );
        }

        for (uint32_t i = count; i < mRenderTargetsCount; ++ i)
        {
            mRenderTargets[i].reset();
        }

        mRenderTargetsCount = count;
    }

    void Context::BeginScene()
    {
        mDevice.lock()->LockContext( shared_from_this() );
        mDrawingContext->MakeCurrent(mContextGL);

        if (mDelayedClearState)
        {
            ClearState();
            mDelayedClearState = false;
        }
    }

    void Context::EndScene()
    {
        ClearState();

        mDrawingContext->MakeCurrent(NULL);
        mDevice.lock()->UnlockContext( shared_from_this() );
    }

} // namespace GFW
