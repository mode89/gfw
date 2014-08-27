#include "cmn/platform.h"
#include "cmn/trace.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4242 4244 4265 4310 4371 4619 4625 4626 )
CMN_WARNING_DISABLE_GCC( unused-local-typedefs )
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/utility.hpp"
#include "boost/serialization/vector.hpp"
CMN_WARNING_POP

#include "cmn/crc32.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/shader.h"
#include "gfw/runtime/ogl4/shader_reflect.h"
#include "gfw/runtime/ogl4/shader_stage.h"
#include "gfw/runtime/ogl4/shader_utils.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/shared/ogl4/shader.h"
#include <functional>
#include <sstream>

namespace GFW {

    using namespace Cmn;

    Shader::Shader( const void * binary, ShaderStage stage, DeviceIn device )
        : ADeviceChild( device )
        , mStage( stage )
        , mHandle( 0 )
        , mHash( 0 )
    {
        const ShaderBinary * shaderBinary = static_cast< const ShaderBinary * >( binary );
        std::istringstream archiveStream( shaderBinary->mData );
        boost::archive::text_iarchive archive( archiveStream );
        ShaderBinaryOgl4 shaderBinaryOgl4;
        archive >> shaderBinaryOgl4;

        const std::string & source = shaderBinaryOgl4.mSource;
        uint32_t shader = CompileShader( mStage, source.c_str() );

        uint32_t program = VGL( glCreateProgram );
        CMN_ASSERT( program != 0 );

        VGL( glProgramParameteri, program, GL_PROGRAM_SEPARABLE, GL_TRUE );
        VGL( glAttachShader, program, shader );

        LinkProgram( program );

        mHandle = program;

        std::hash< std::string > hasher;
        mHash = hasher( source );

        mReflection = std::make_shared< ShaderReflection >( shaderBinaryOgl4, mHandle, device );

#if CMN_DEBUG
CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4996 )
        mSource = source;
CMN_WARNING_POP
#endif
    }

    Shader::~Shader()
    {
        if (mHandle != 0)
        {
            VGL( glDeleteProgram, mHandle );
        }
    }

} // namespace GFW
