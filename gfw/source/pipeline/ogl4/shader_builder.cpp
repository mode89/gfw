#include "common/trace.h"
#include "gfw/pipeline/shader_builder.h"

namespace GFW {

    ShaderBuilder::ShaderBuilder( const ParseTree * tree )
        : mParseTree( tree )
    {

    }

    ShaderBuilder::~ShaderBuilder()
    {

    }

    ShaderBinaryRef ShaderBuilder::Compile( const char * shaderName, const char * profile )
    {
        return NULL;
    }

} // namespace GFW
