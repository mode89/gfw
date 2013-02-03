#include "profiler\profiler.h"

#include "common\trace.h"
#include "common\crc32.h"

#include "gfw\allocator.h"
#include "gfw\graphics\common\format.h"
#include "gfw\graphics\opengl\context.h"
#include "gfw\graphics\opengl\functions.h"

namespace GFW { namespace OpenGL {

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

    Context::Context(Platform::IWindowIn window, DeviceIn d, IAllocator * a)
        : mDevice(d)
    {
        mAllocator = a;

        mWindow = mDevice->GetPlatform()->CreateOpenglWindow(window).StaticCast<OpenglWindow>();

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
        TRACE_ASSERT(stage < SHADER_STAGE_NUMBER);
        TRACE_ASSERT(shader.IsAttached());
        TRACE_ASSERT(stage == shader->GetStage());

        if (stage == shader->GetStage())
        {
            mShaders[stage] = shader.StaticCast<Shader>();
        }
    }

    void Context::Clear(const ClearParams & cp)
    {
        PROFILE();

        uint32_t mask = 0;

        if (cp.mask | CLEAR_COLOR)
        {
            TRACE_ASSERT_GL(glClearColor, cp.color[0], cp.color[1], cp.color[2], cp.color[3]);
            mask |= GL_COLOR_BUFFER_BIT;
        }

        TRACE_ASSERT_GL(glClear, mask);

#if PROFILING
        TRACE_ASSERT_GL(glFlush);
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
        TRACE_ASSERT_GL(glFlush);
        TRACE_ASSERT_GL(glFinish);
#endif
    }

    void Context::Present()
    {
        PROFILE();

        mWindow->SwapBuffers();

        ClearState();
    }

    void Context::ClearState()
    {
        PROFILE();

        // Detach shaders

        TRACE_ASSERT_GL(glUseProgram, 0);

        for (int i = 0; i < SHADER_STAGE_NUMBER; ++ i)
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

        uint32_t shaderHashes[SHADER_STAGE_NUMBER];
        for (int stage = 0; stage < SHADER_STAGE_NUMBER; ++ stage)
        {
            TRACE_ASSERT(mShaders[stage].IsAttached());
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

            for (int stage = 0; stage < SHADER_STAGE_NUMBER; ++ stage)
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

                char * infoLog = GFW_NEW_ARRAY(mAllocator, char8_t, infoLogLength + 1);
                TRACE_ASSERT_GL(glGetProgramInfoLog, program, infoLogLength, NULL, infoLog);

                TRACE_ERROR_FORMATTED("Cannot link the program\n\n%s\n", infoLog);

                GFW_DELETE(mAllocator, infoLog);

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
        TRACE_ASSERT_GL(glFlush);
        TRACE_ASSERT_GL(glFinish);
#endif
    }

}} // namespace GFW::OpenGL
