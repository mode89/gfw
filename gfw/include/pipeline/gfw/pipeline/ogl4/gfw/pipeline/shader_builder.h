#ifndef __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
#define __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__

#include "gfw/pipeline/common/shader_builder.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace GFW {

    class ParseTree;

    class ShaderBuilder : public IShaderBuilder
    {
    public:
        ShaderBinaryRef
        Compile( const char * shaderName, const char * profile );

    public:
        ShaderBuilder( ConstParseTreeIn );
        ~ShaderBuilder();

    private:
        bool
        CollectFunctions( ConstParseTreeIn );

    private:
        typedef std::vector< ConstParseTreeRef > ParseTreeMap;

        struct Function
        {
            ConstParseTreeRef   tree;
            ConstParseTreeRef   ret;
            ConstParseTreeRef   sem;
            ParseTreeMap        args;

            Function()
                : tree( NULL )
                , ret( NULL )
                , sem( NULL )
            {}
        };

        typedef std::unordered_map< std::string, Function > FunctionMap;

        ConstParseTreeRef   mParseTree;

        FunctionMap         mFunctions;
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBuilder );

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
