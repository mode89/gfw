#include "common/trace.h"
#include "common/crc32.h"

#include "gfw/core/shader.h"
#include "gfw/core/shader_reflect.h"
#include "gfw/core/shader_stage.h"
#include "gfw/core/functions.h"

#include "gfw/common/device_child.inl"

#include <string.h>

namespace GFW {

    using namespace Common;

    Shader::Shader( const char * source, ShaderStage stage, IDeviceRef device )
        : ADeviceChild(device)
        , mStage(stage)
        , mHandle(0)
        , mHash(0)
    {
        uint32_t shader = TRACE_ASSERT_GL(glCreateShader, GetOGLShaderType(mStage));
        TRACE_ASSERT(shader != 0);

        const char * strings[] = { source };
        TRACE_ASSERT_GL(glShaderSource, shader, 1, strings, NULL);

        TRACE_ASSERT_GL(glCompileShader, shader);

        int32_t compileStatus = 0;
        TRACE_ASSERT_GL(glGetShaderiv, shader, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE)
        {
            int32_t infoLogLength = 0;
            TRACE_ASSERT_GL(glGetShaderiv, shader, GL_INFO_LOG_LENGTH, &infoLogLength);

            char * infoLog = new char [infoLogLength + 1];
            TRACE_ASSERT_GL(glGetShaderInfoLog, shader, infoLogLength, NULL, infoLog);

            infoLog[infoLogLength] = 0;
            TRACE_ERROR_FORMATTED("Cannot compile the shader\n\n%s\n", infoLog);

            delete infoLog;

            TRACE_ASSERT_GL(glDeleteShader, shader);

            return;
        }

        uint32_t program = TRACE_ASSERT_GL(glCreateProgram);
        TRACE_ASSERT(program != 0);

        TRACE_ASSERT_GL(glProgramParameteri, program, GL_PROGRAM_SEPARABLE, GL_TRUE);
        TRACE_ASSERT_GL(glAttachShader, program, shader);
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

            TRACE_ASSERT_GL(glDeleteShader, shader);
            TRACE_ASSERT_GL(glDeleteProgram, program);

            return;
        }

        mHandle = program;

        uint32_t sourceLength = strlen(source);
        mHash = CRC32(0, source, sourceLength);

        mReflection = new ShaderReflection(mHandle, mDevice);
    }

    Shader::~Shader()
    {
        if (mHandle != 0)
        {
            TRACE_ASSERT_GL(glDeleteProgram, mHandle);
        }
    }

} // namespace GFW
