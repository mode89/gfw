#include "common/trace.h"
#include "common/crc32.h"

#include "gfw/base/render_buffer.h"

#include "gfw/common/format.h"
#include "gfw/common/semantic.h"

#include "gfw/core/buffer.h"
#include "gfw/core/context.h"
#include "gfw/core/device.h"
#include "gfw/core/drawing_context.h"
#include "gfw/core/format.h"
#include "gfw/core/functions.h"
#include "gfw/core/shader.h"
#include "gfw/core/texture.h"

namespace GFW {

    using namespace Common;

    Context::Context(DeviceIn d, IDrawingContextIn dc)
        : mDevice(d)
        , mDrawingContext(dc)
        , mContextGL(NULL)
        , mScreenQuadBuffer(0)
        , mDelayedClearState(false)
        , mActiveTexturesDirtyMask(0)
        , mNextActiveTextureUnit(0)
    {
        mContextGL = mDrawingContext->CreateContext();

        InitScreenQuad();

        ClearState();
    }

    Context::~Context()
    {
        for (tMapProgs::iterator it = mPrograms.begin(); it != mPrograms.end(); ++ it)
        {
            if (it->second != 0)
            {
                TRACE_ASSERT_GL(glDeleteProgram, it->second);
            }
        }

        TRACE_ASSERT(mContextGL != NULL);
        mDrawingContext->DeleteContext(mContextGL);

        TRACE_ASSERT(mScreenQuadBuffer != 0);
        TRACE_ASSERT_GL(glDeleteBuffers, 1, &mScreenQuadBuffer);
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

    void Context::SetVertexAttributes( uint32_t number, VertexAttribute attr[] )
    {
        TRACE_ASSERT(number > 0);
        TRACE_ASSERT(number <= MAX_VERTEX_BUFFER_BIND);

        for (uint32_t i = 0; i < number; ++ i)
        {
            mVertAttrs[i] = attr[i];
        }
    }

    void Context::SetVertexBuffer( uint32_t slot, IBufferIn buf )
    {
        TRACE_ASSERT(slot >= 0);
        TRACE_ASSERT(slot < MAX_VERTEX_BUFFER_BIND);
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

        TRACE_ASSERT_GL(glUseProgram, 0);

        for (int i = 0; i < SHADER_STAGE_COUNT; ++ i)
        {
            mShaders[i].Detach();
        }

        // Detach buffers

        for (int i = 0; i < MAX_VERTEX_BUFFER_BIND; ++ i)
        {
            mVertexBuffers[i].Detach();
        }

        TRACE_ASSERT_GL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
        mIndexBuffer.Detach();

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
    }

    void Context::FlushState()
    {
        // Setup shaders

        uint32_t shaderHashes[SHADER_STAGE_COUNT];
        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            shaderHashes[stage] = mShaders[stage].IsAttached() ? mShaders[stage]->GetHash() : 0;
        }

        uint32_t programHash = CRC32(0, shaderHashes, sizeof(shaderHashes));

        uint32_t program = 0;
        tMapProgs::iterator it = mPrograms.find(programHash);
        if (it != mPrograms.end())
        {
            program = it->second;
        }
        else
        {
            program = TRACE_ASSERT_GL(glCreateProgram);
            TRACE_ASSERT(program != 0);

            for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
            {
                if (mShaders[stage].IsAttached())
                {
                    TRACE_ASSERT_GL(glAttachShader, program, mShaders[stage]->GetShaderObject());
                }
            }

            TRACE_ASSERT_GL(glLinkProgram, program);

            int32_t linkStatus = 0;
            TRACE_ASSERT_GL(glGetProgramiv, program, GL_LINK_STATUS, &linkStatus);

            if (linkStatus == 0)
            {
                int32_t infoLogLength = 0;
                TRACE_ASSERT_GL(glGetProgramiv, program, GL_INFO_LOG_LENGTH, &infoLogLength);

                char * infoLog = new char8_t [infoLogLength + 1];
                TRACE_ASSERT_GL(glGetProgramInfoLog, program, infoLogLength, NULL, infoLog);

                TRACE_ERROR_FORMATTED("Cannot link the program\n\n%s\n", infoLog);

                delete infoLog;

                TRACE_ASSERT_GL(glDeleteProgram, program);
                program = 0;
            }

            mPrograms[programHash] = program;
        }

        TRACE_ASSERT_GL(glUseProgram, program);

        // Bind attributes

        for (int i = 0; i < MAX_VERTEX_BUFFER_BIND; ++ i)
        {
            TRACE_ASSERT_GL(glDisableVertexAttribArray, i);
        }

        for (int i = 0; i < MAX_VERTEX_BUFFER_BIND; ++ i)
        {
            const VertexAttribute & attr = mVertAttrs[i];

            if (attr.semantic != SEMANTIC_UNKNOWN)
            {
                if (mVertexBuffers[attr.bufSlot].IsAttached())
                {
                    int32_t attrIndex = TRACE_ASSERT_GL(glGetAttribLocation, program, GetSemanticString(attr.semantic));
                    TRACE_ASSERT(attrIndex != -1);

                    if (attrIndex != -1)
                    {
                        uint32_t bufObj = mVertexBuffers[attr.bufSlot]->GetHandle();
                        TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, bufObj);

                        int32_t size = GetFormatElementNumber(attr.format);
                        Type type    = GetFormatElementType(attr.format);
                        TRACE_ASSERT_GL(glVertexAttribPointer, attrIndex, size, GetOGLType(type), GL_FALSE, attr.stride, reinterpret_cast<void*>(attr.offset));

                        TRACE_ASSERT_GL(glEnableVertexAttribArray, attrIndex);

                        TRACE_ASSERT_GL(glBindBuffer, GL_ARRAY_BUFFER, 0);
                    }
                }
            }
        }

        if (mIndexBuffer.IsAttached())
        {
            TRACE_ASSERT_GL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer->GetHandle());
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

    void Context::SetFrameBuffer( uint32_t colorBufferCount, IRenderBufferRef color[], IRenderBufferIn depth )
    {
        TRACE_FAIL_MSG("Not yet implemented");
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
