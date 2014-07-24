#include "common/trace.h"
#include "common/crc32.h"
#include "gfw/runtime/core/device.h"
#include "gfw/runtime/core/shader.h"
#include "gfw/runtime/core/shader_reflect.h"
#include "gfw/runtime/core/shader_stage.h"
#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/common/device_child.inl"

#include <cstring>

namespace GFW {

    using namespace Common;

    Shader::Shader( const void * binary, ShaderStage stage, DeviceIn device )
        : ADeviceChild( device )
        , mStage( stage )
        , mHandle( 0 )
        , mHash( 0 )
#if PLAT_DEBUG
        , mSource( nullptr )
#endif
    {
        uint32_t shader = TRACE_ASSERT_GL( glCreateShader, GetOGLShaderType( stage ) );
        TRACE_ASSERT( shader != 0 );

        const ShaderBinary * shaderBinary = static_cast< const ShaderBinary * >( binary );
        const char * source = reinterpret_cast< const char * >( shaderBinary->mData.data() );
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
            TRACE_ERR("Cannot compile the shader\n\n%s\n", infoLog);

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

            TRACE_ERR("Cannot link the program\n\n%s\n", infoLog);

            delete infoLog;

            TRACE_ASSERT_GL(glDeleteShader, shader);
            TRACE_ASSERT_GL(glDeleteProgram, program);

            return;
        }

        mHandle = program;

        uint32_t sourceLength = strlen(source);
        mHash = CRC32(0, source, sourceLength);

        mReflection = std::make_shared<ShaderReflection>( mHandle, device );

#if PLAT_DEBUG
#pragma warning( push )
#pragma warning( disable : 4996 )
        mSource = new char [ std::strlen( source ) + 1 ];
        std::strcpy( mSource, source );
#pragma warning( pop )
#endif
    }

    Shader::~Shader()
    {
        if (mHandle != 0)
        {
            TRACE_ASSERT_GL(glDeleteProgram, mHandle);
        }

#if PLAT_DEBUG
        if ( mSource != NULL )
        {
            delete[] mSource;
        }
#endif
    }

} // namespace GFW
