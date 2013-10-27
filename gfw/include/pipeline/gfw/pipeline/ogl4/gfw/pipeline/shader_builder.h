#ifndef __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
#define __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__

#include "common/autoref.h"
#include "gfw/pipeline/common/shader_binary.h"

#include <string>
#include <unordered_map>
#include <vector>

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
        bool
        CollectFunctions( const ParseTree * );

    private:
        typedef std::vector< const ParseTree * > ParseTreeMap;

        struct Function
        {
            const ParseTree *   tree;
            const ParseTree *   ret;
            const ParseTree *   sem;
            ParseTreeMap        args;

            Function()
                : tree( NULL )
                , ret( NULL )
                , sem( NULL )
            {}
        };

        typedef std::unordered_map< std::string, Function > FunctionMap;

        const ParseTree *   mParseTree;

        FunctionMap         mFunctions;
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBuilder );

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
