#include "profiler/profiler.h"

#include "common/trace.h"
#include "common/crc32.h"

#include "gfw/common/format.h"

#include "gfw/ogl/context.h"
#include "gfw/ogl/functions.h"

namespace GFW {

    using namespace Common;

    static uint32_t GetOGLType(Type type)
    {
        switch (type)
        {
        case TYPE_FLOAT:    return GL_FLOAT;
        default:            TRACE_FAIL();
        }

        return 0;
    }

    static uint32_t GetDrawMode(Primitive prim)
    {
        switch (prim)
        {
        case PRIM_TRIANGLES:    return GL_TRIANGLES;
        default:                TRACE_FAIL();
        }

        return 0;
    }

    Context::Context(DeviceIn d, IDrawingContextIn dc)
        : mDevice(d)
        , mDrawingContext(dc)
        , mContextGL(NULL)
    {
        mContextGL = mDrawingContext->CreateContext();

        memset(mVertAttrs, 0, sizeof(mVertAttrs));
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

    void Context::SetShader( ShaderStage stage, IShaderIn shader )
    {
        TRACE_ASSERT(stage > SHADER_UNKNOWN);
        TRACE_ASSERT(stage < SHADER_STAGE_COUNT);
        TRACE_ASSERT(shader.IsAttached());
        TRACE_ASSERT(stage == shader->GetStage());

        mShaders[stage] = shader.StaticCast<Shader>();
    }

    void Context::Clear(const ClearParams & cp)
    {
        PROFILE();

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

#if PROFILING
        TRACE_ASSERT_GL(glFinish);
#endif
    }

    void Context::Draw( const DrawParams & dp )
    {
        PROFILE();

        FlushState();

        uint32_t mode = GetDrawMode(dp.primTop);
        TRACE_ASSERT_GL(glDrawArrays, mode, dp.vertexStart, dp.vertexCount);

#if PROFILING
        TRACE_ASSERT_GL(glFinish);
#endif
    }

    void Context::Draw( const DrawIndexedParams & )
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Context::ClearState()
    {
        PROFILE();

        // Detach shaders

        TRACE_ASSERT_GL(glUseProgram, 0);

        for (int i = 0; i < SHADER_STAGE_COUNT; ++ i)
        {
            mShaders[i].Detach();
        }

        // Detach vertex buffers

        for (int i = 0; i < MAX_VERTEX_BUFFER_BIND; ++ i)
        {
            mVertexBuffers[i].Detach();
        }
    }

    void Context::FlushState()
    {
        PROFILE();

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

            if (attr.name != NULL)
            {
                if (mVertexBuffers[attr.bufSlot].IsAttached())
                {
                    int32_t attrIndex = TRACE_ASSERT_GL(glGetAttribLocation, program, attr.name);
                    TRACE_ASSERT(attrIndex != -1);

                    if (attrIndex != -1)
                    {
                        uint32_t bufObj = mVertexBuffers[attr.bufSlot]->GetBufferObject();
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

#if PROFILING
        TRACE_ASSERT_GL(glFinish);
#endif
    }

    void Context::SetIndexBuffer( IBufferIn )
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Context::SetTexture( ShaderStage, uint32_t slot, ITextureIn )
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Context::SetFrameBuffer( uint32_t colorBufferCount, IRenderBufferRef color[], IRenderBufferIn depth )
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Context::DrawScreenQuad()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Context::BeginScene()
    {
        mDrawingContext->MakeCurrent(mContextGL);
    }

    void Context::EndScene()
    {
        mDrawingContext->MakeCurrent(NULL);
    }

} // namespace GFW
