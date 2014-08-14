#include "cmn/trace.h"
#include "gfw/shader_stage.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/shader_stage.h"
#include "gfw/runtime/ogl4/shader_utils.h"

namespace GFW {

    uint32_t CompileShader( ShaderStage stage, const char * source )
    {
        uint32_t shader = VGL( glCreateShader, GetOGLShaderType( stage ) );
        CMN_ASSERT( shader != 0 );

        VGL( glShaderSource, shader, 1, &source, NULL );
        VGL( glCompileShader, shader );

        int32_t compileStatus = 0;
        VGL( glGetShaderiv, shader, GL_COMPILE_STATUS, &compileStatus );

        if ( compileStatus == GL_FALSE )
        {
            int32_t infoLogLength = 0;
            VGL( glGetShaderiv, shader, GL_INFO_LOG_LENGTH, &infoLogLength );

            char * infoLog = new char [ infoLogLength + 1 ];
            VGL( glGetShaderInfoLog, shader, infoLogLength, NULL, infoLog );

            infoLog[infoLogLength] = 0;
            CMN_FAIL_MSG( "Cannot compile the shader\n\n%s\n", infoLog );

            delete infoLog;

            VGL( glDeleteShader, shader );

            return 0;
        }

        return shader;
    }

    void LinkProgram( uint32_t program )
    {
        VGL( glLinkProgram, program );

        int32_t linkStatus = 0;
        VGL( glGetProgramiv, program, GL_LINK_STATUS, &linkStatus );

        if (linkStatus == 0)
        {
            int32_t infoLogLength = 0;
            VGL( glGetProgramiv, program, GL_INFO_LOG_LENGTH, &infoLogLength );

            char * infoLog = new char [ infoLogLength + 1 ];
            VGL( glGetProgramInfoLog, program, infoLogLength, NULL, infoLog );

            CMN_FAIL_MSG( "Cannot link the program\n\n%s\n", infoLog );

            delete infoLog;

            VGL( glDeleteProgram, program );
        }
    }

} // namespace GFW
