#ifndef __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
#define __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__

#include "gfw/pipeline/common/shader_builder.h"
#include "gfw/pipeline/common/symbol_table.h"

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace GFW {

    class ParseTree;

    class ShaderBuilder : public IShaderBuilder
    {
    public:
        typedef std::pair< const Symbol *, const Symbol * > TextureSamplerPair;
        typedef std::set< TextureSamplerPair > TextureSamplerPairSet;

    public:
        ShaderBinaryRef
        Compile( const char * shaderName, const char * profile );

    public:
        ShaderBuilder( ConstParseTreeIn, ConstSymbolTableIn );
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
        typedef std::unordered_map< const Symbol *, TextureSamplerPairSet > FunctionTextureSamplerMap;

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

        ConstParseTreeRef           mParseTree;
        ConstSymbolTableIn          mSymbolTable;

        FunctionMap                 mFunctions;
        ParseTreeVec                mFXNodes;
        ParseTreeMap                mVariables;
        FunctionTextureSamplerMap   mFunctionTextureSamplerMap;
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBuilder );

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
