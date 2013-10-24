#ifndef __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
#define __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__

#include "common/autoref.h"
#include "gfw/pipeline/common/shader_binary.h"

namespace GFW {

    class ParseTree;

    class ShaderBuilder : public Common::ARefCounted
    {
    public:
        ShaderBinaryRef
        Compile( const char * shaderName, const char * profile );

    public:
        ShaderBuilder( const ParseTree * );
        ~ShaderBuilder();

    private:
        const ParseTree *   mParseTree;
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBuilder );

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
