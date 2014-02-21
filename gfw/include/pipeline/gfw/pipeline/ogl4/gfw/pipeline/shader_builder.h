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

        bool
        CollectFXNodes( ConstParseTreeIn );

        bool
        CollectVariables( ConstParseTreeIn );

    private:
        struct Function;

        typedef std::vector< ConstParseTreeRef > ParseTreeVec;
        typedef std::unordered_map< std::string, ConstParseTreeRef > ParseTreeMap;
        typedef std::unordered_map< std::string, Function > FunctionMap;

        struct Function
        {
            ConstParseTreeRef   tree;
            ConstParseTreeRef   ret;
            ConstParseTreeRef   sem;
            ParseTreeVec        args;

            Function()
                : tree( NULL )
                , ret( NULL )
                , sem( NULL )
            {}
        };

        ConstParseTreeRef   mParseTree;

        FunctionMap         mFunctions;
        ParseTreeVec        mFXNodes;
        ParseTreeMap        mVariables;
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBuilder );

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
