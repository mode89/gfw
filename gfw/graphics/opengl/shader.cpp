#include "common/trace.h"
#include "common/crc32.h"

#include "gfw/allocator.h"
#include "gfw/graphics/opengl/shader.h"
#include "gfw/graphics/opengl/functions.h"

#include <string.h>

namespace GFW { namespace OpenGL {

    using namespace Common;

    static uint32_t GetShaderType(ShaderStage stage)
    {
        switch (stage)
        {
        case SHADER_VERTEX:     return GL_VERTEX_SHADER;
        case SHADER_PIXEL:      return GL_FRAGMENT_SHADER;
        default:                TRACE_FAIL();
        }

        return 0;
    }

    Shader::Shader( ShaderStage stage, Common::IAllocator * a )
        : mStage(stage)
        , mShader(0)
        , mHash(0)
    {
        mAllocator = a;
    }

    Shader::~Shader()
    {
        if (mShader != 0)
        {
            TRACE_ASSERT_GL(glDeleteShader, mShader);
        }
    }

    uint32_t Shader::Compile( const char * source )
    {
        mShader = TRACE_ASSERT_GL(glCreateShader, GetShaderType(mStage));
        TRACE_ASSERT(mShader != 0);

        if (mShader != 0)
        {
            const char * strings[] = { source };
            TRACE_ASSERT_GL(glShaderSource, mShader, 1, strings, NULL);

            TRACE_ASSERT_GL(glCompileShader, mShader);

            int32_t compileStatus = 0;
            TRACE_ASSERT_GL(glGetShaderiv, mShader, GL_COMPILE_STATUS, &compileStatus);

            if (compileStatus == GL_TRUE)
            {
                uint32_t sourceLength = strlen(source);
                mHash = CRC32(0, source, sourceLength);
                return 1;
            }

            int32_t infoLogLength = 0;
            TRACE_ASSERT_GL(glGetShaderiv, mShader, GL_INFO_LOG_LENGTH, &infoLogLength);

            char * infoLog = GFW_NEW_ARRAY(mAllocator, char, infoLogLength + 1);
            TRACE_ASSERT_GL(glGetShaderInfoLog, mShader, infoLogLength, NULL, infoLog);

            infoLog[infoLogLength] = 0;
            TRACE_ERROR_FORMATTED("Cannot compile the shader\n\n%s\n", infoLog);

            GFW_DELETE(mAllocator, infoLog);

            TRACE_ASSERT_GL(glDeleteShader, mShader);
            mShader = 0;
        }

        return 0;
    }

}} // namespace GFW::OpenGL
