#ifndef __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
#define __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace GFW {

    class ParseTree;
    class ShaderBinary;
    class Symbol;
    class SymbolTable;

    typedef std::pair< const Symbol *, const Symbol * > TextureSamplerPair;
    typedef std::set< TextureSamplerPair > TextureSamplerPairSet;

    class ShaderBuilder
    {
    public:
        void
        Build( ShaderBinary &, const std::string & entryPoint, const std::string & profile );

    public:
        ShaderBuilder( const ParseTree &, const SymbolTable & );
        ~ShaderBuilder();

    private:
        bool
        CollectVariables( const ParseTree & );

    private:
        typedef std::unordered_map< std::string, const ParseTree * > ParseTreeMap;
        typedef std::unordered_map< const Symbol *, TextureSamplerPairSet > FunctionTextureSamplerMap;

        const ParseTree &           mParseTree;
        const SymbolTable &         mSymbolTable;

        ParseTreeMap                mVariables;
        FunctionTextureSamplerMap   mFunctionTextureSamplerMap;
    };

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
