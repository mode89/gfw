#include "common/trace.h"
#include "common/crc32.h"

#include "gfw/common/format.h"
#include "gfw/common/semantic.h"

#include "gfw/core/buffer.h"
#include "gfw/core/context.h"
#include "gfw/core/device.h"
#include "gfw/core/drawing_context.h"
#include "gfw/core/format.h"
#include "gfw/core/functions.h"
#include "gfw/core/input_layout.h"
#include "gfw/core/render_target.h"
#include "gfw/core/shader.h"
#include "gfw/core/shader_reflect.h"
#include "gfw/core/shader_stage.h"
#include "gfw/core/texture.h"

namespace GFW {

    using namespace Common;

    Context::Context(IDrawingContextIn dc, Device * d)
        : mDevice(d)
        , mDrawingContext(dc)
        , mContextGL(NULL)
        , mScreenQuadBuffer(0)
        , mProgramPipeline(0)
        , mEnabledVertexAttributesMask(0)
        , mDelayedClearState(false)
        , mActiveTexturesDirtyMask(0)
        , mNextActiveTextureUnit(0)
        , mRenderTargetsCount(0)
        , mDrawFramebuffer(0)
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

    void Context::SetInputLayout(IInputLayoutIn layout)
    {
        mInputLayout = layout;
    }

    void Context::SetVertexBuffer( uint32_t slot, IBufferIn buf )
    {
        TRACE_ASSERT(slot >= 0);
        TRACE_ASSERT(slot < MAX_BIND_VERTEX_BUFFERS);
        TRACE_ASSERT(buf.IsAttached());

        mVertexBuffers[slot] = buf.StaticCast<Buffer>();
    }

    void Context::SetShader( int32_t stage, IShaderIn shader )
    {
        TRACE_ASSERT(stage > SHADER_STAGE_UNKNOWN);
        TRACE_ASSERT(stage < SHADER_STAGE_COUNT);
        TRACE_ASSERT(shader.IsAttached());
        TRACE_ASSERT(stage == shader->GetStage());

        mShaders[stage] = shader.StaticCast<Shader>();
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
        if (mDevice->GetCurrentContext().GetPointer() != this)
        {
            mDelayedClearState = true;
            return;
        }

        // Detach shaders

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            uint32_t stageBit = GetOGLShaderStageBit(static_cast<ShaderStage>(stage));
            TRACE_ASSERT_GL(glUseProgramStages, mProgramPipeline, stageBit, 0);

            mShaders[stage].Detach();
        }

        // Detach buffers

        for (int i = 0; i < MAX_BIND_VERTEX_BUFFERS; ++ i)
        {
            mVertexBuffers[i].Detach();
        }

        TRACE_ASSERT_GL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
        mIndexBuffer.Detach();

        for (uint32_t attrIndex = 0, mask = mEnabledVertexAttributesMask; mask; ++ attrIndex, mask >>= 1)
        {
            if (mask & 1)
            {
                TRACE_ASSERT(attrIndex < MAX_INPUT_ATTRIBUTES);
                TRACE_ASSERT_GL(glDisableVertexAttribArray, attrIndex);
            }
        }
        mEnabledVertexAttributesMask = 0;

        mInputLayout.Detach();

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
            if (mActiveTextures[texUnit].IsAttached())
            {
                TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + texUnit);
                TRACE_ASSERT_GL(glBindTexture, GL_TEXTURE_2D, 0);
                mActiveTextures[texUnit].Detach();
            }
        }

        mActiveTexturesDirtyMask = 0;
        mNextActiveTextureUnit   = 0;

        TRACE_ASSERT_GL(glActiveTexture, GL_TEXTURE0 + MAX_BIND_TEXTURES); // Is used as a temp texture

        // Detach render targets

        for (uint32_t i = 0; i < mRenderTargetsCount; ++ i)
        {
            TRACE_ASSERT_GL(glFramebufferTexture, GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 0, 0);
            mRenderTargets[i].Detach();
        }
        TRACE_ASSERT_GL(glDrawBuffer, GL_NONE);
        mRenderTargetsCount = 0;
    }

    void Context::FlushState()
    {
        // Setup shaders

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            ShaderRef shader = mShaders[stage];
            uint32_t stageBit = GetOGLShaderStageBit(static_cast<ShaderStage>(stage));
            TRACE_ASSERT_GL(glUseProgramStages, mProgramPipeline, stageBit, shader.IsAttached() ? shader->GetHandle() : 0);
        }

        // Bind buffers

        if (mInputLayout.IsAttached())
        {
            for (uint32_t attrIndex = 0, mask = mInputLayout->GetEnabledAttributesMask(); mask; ++ attrIndex, mask >>= 1)
            {
                if (mask & 1)
                {
                    TRACE_ASSERT(attrIndex < MAX_INPUT_ATTRIBUTES);

                    const VertexAttribute & attr = mInputLayout->GetAttribute(attrIndex);
                    TRACE_ASSERT(attr.semantic != SEMANTIC_UNKNOWN);
                    TRACE_ASSERT(mVertexBuffers[attr.bufSlot].IsAttached());

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

            if (mIndexBuffer.IsAttached())
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
            RenderTargetRef rt      = mRenderTargets[i];
            TextureRef      rtTex   = rt->GetTexture();
            TRACE_ASSERT_GL(glFramebufferTexture, GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rtTex->GetHandle(), rt->GetDesc().resourceIndex);
            drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        TRACE_ASSERT_GL(glDrawBuffers, mRenderTargetsCount, drawBuffers);

#ifdef TRACE_ASSERT_ENABLED
        int32_t status  = TRACE_ASSERT_GL(glCheckFramebufferStatus, GL_DRAW_FRAMEBUFFER);
        TRACE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);
#endif
    }

    void Context::SetIndexBuffer( IBufferIn buffer )
    {
        mIndexBuffer = buffer;
    }

    void Context::SetTexture( int32_t stage, uint32_t slot, ITextureIn texture )
    {
        TRACE_ASSERT(stage >= 0);
        TRACE_ASSERT(stage < SHADER_STAGE_COUNT);
        TRACE_ASSERT(slot < MAX_BIND_TEXTURES);
        TRACE_ASSERT(texture.IsAttached());

        int32_t & textureUnit = mTextureUnits[stage][slot];

        if (textureUnit == -1)
        {
            TRACE_ASSERT(mActiveTextures[mNextActiveTextureUnit].IsNull());

            textureUnit = mNextActiveTextureUnit;
            mActiveTextures[mNextActiveTextureUnit] = texture;
            mActiveTexturesDirtyMask |= (1 << mNextActiveTextureUnit);

            // Find a free texture unit

            uint32_t unit = (mNextActiveTextureUnit + 1) % MAX_BIND_TEXTURES;
            while (mActiveTextures[unit].IsAttached() && unit != mNextActiveTextureUnit)
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

                mActiveTextures[textureUnit] = texture;
                mActiveTexturesDirtyMask |= (1 << textureUnit);
            }
        }
    }

    void Context::SetRenderTargets( uint32_t count, IRenderTargetRef rt[] )
    {
        for (uint32_t i = 0; i < count; ++ i)
        {
            mRenderTargets[i] = rt[i];
        }

        for (uint32_t i = count; i < mRenderTargetsCount; ++ i)
        {
            mRenderTargets[i].Detach();
        }

        mRenderTargetsCount = count;
    }

    void Context::BeginScene()
    {
        mDevice->LockContext(this);
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
        mDevice->UnlockContext(this);
    }

} // namespace GFW
