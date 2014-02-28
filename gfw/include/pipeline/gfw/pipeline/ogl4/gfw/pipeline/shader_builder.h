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
        CollectVariables( ConstParseTreeIn );

    private:
        typedef std::unordered_map< std::string, ConstParseTreeRef > ParseTreeMap;
        typedef std::unordered_map< const Symbol *, TextureSamplerPairSet > FunctionTextureSamplerMap;

        ConstParseTreeRef           mParseTree;
        ConstSymbolTableRef         mSymbolTable;

        ParseTreeMap                mVariables;
        FunctionTextureSamplerMap   mFunctionTextureSamplerMap;
    };
    AUTOREF_REFERENCE_DECLARATION( ShaderBuilder );

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
