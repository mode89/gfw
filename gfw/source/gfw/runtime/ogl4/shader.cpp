#include "cmn/trace.h"
#include "cmn/crc32.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/shader.h"
#include "gfw/runtime/ogl4/shader_reflect.h"
#include "gfw/runtime/ogl4/shader_stage.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/common/device_child.inl"

#include <cstring>

namespace GFW {

    using namespace Cmn;

    Shader::Shader( const void * binary, ShaderStage stage, DeviceIn device )
        : ADeviceChild( device )
        , mStage( stage )
        , mHandle( 0 )
        , mHash( 0 )
#if CMN_DEBUG
        , mSource( nullptr )
#endif
    {
        uint32_t shader = VGL( glCreateShader, GetOGLShaderType( stage ) );
        CMN_ASSERT( shader != 0 );

        const ShaderBinary * shaderBinary = static_cast< const ShaderBinary * >( binary );
        const char * source = reinterpret_cast< const char * >( shaderBinary->mData.data() );
        const char * strings[] = { source };
        VGL( glShaderSource, shader, 1, strings, NULL );

        VGL( glCompileShader, shader );

        int32_t compileStatus = 0;
        VGL( glGetShaderiv, shader, GL_COMPILE_STATUS, &compileStatus );

        if (compileStatus == GL_FALSE)
        {
            int32_t infoLogLength = 0;
            VGL( glGetShaderiv, shader, GL_INFO_LOG_LENGTH, &infoLogLength );

            char * infoLog = new char [infoLogLength + 1];
            VGL( glGetShaderInfoLog, shader, infoLogLength, NULL, infoLog );

            infoLog[infoLogLength] = 0;
            CMN_ERR( "Cannot compile the shader\n\n%s\n", infoLog );

            delete infoLog;

            VGL( glDeleteShader, shader );

            return;
        }

        uint32_t program = VGL( glCreateProgram );
        CMN_ASSERT( program != 0 );

        VGL( glProgramParameteri, program, GL_PROGRAM_SEPARABLE, GL_TRUE );
        VGL( glAttachShader, program, shader );
        VGL( glLinkProgram, program );

        int32_t linkStatus = 0;
        VGL( glGetProgramiv, program, GL_LINK_STATUS, &linkStatus );

        if (linkStatus == 0)
        {
            int32_t infoLogLength = 0;
            VGL( glGetProgramiv, program, GL_INFO_LOG_LENGTH, &infoLogLength );

            char * infoLog = new char8_t [infoLogLength + 1];
            VGL( glGetProgramInfoLog, program, infoLogLength, NULL, infoLog );

            CMN_ERR( "Cannot link the program\n\n%s\n", infoLog );

            delete infoLog;

            VGL( glDeleteShader, shader );
            VGL( glDeleteProgram, program );

            return;
        }

        mHandle = program;

        uint32_t sourceLength = strlen(source);
        mHash = CRC32(0, source, sourceLength);

        mReflection = std::make_shared<ShaderReflection>( mHandle, device );

#if CMN_DEBUG
CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4996 )
        mSource = new char [ std::strlen( source ) + 1 ];
        std::strcpy( mSource, source );
CMN_WARNING_POP
#endif
    }

    Shader::~Shader()
    {
        if (mHandle != 0)
        {
            VGL( glDeleteProgram, mHandle );
        }

#if CMN_DEBUG
        if ( mSource != NULL )
        {
            delete[] mSource;
        }
#endif
    }

} // namespace GFW
