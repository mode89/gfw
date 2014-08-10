#ifndef __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
#define __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__

#include "gfw/pipeline/common/shader_builder.h"

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace GFW {

    class ParseTree;
    class ShaderBinary;

    typedef std::pair< const Symbol *, const Symbol * > TextureSamplerPair;
    typedef std::set< TextureSamplerPair > TextureSamplerPairSet;

    class ShaderBuilder : public IShaderBuilder
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

        ShaderBuilder & operator= ( const ShaderBuilder & );

    private:
        typedef std::unordered_map< std::string, const ParseTree * > ParseTreeMap;
        typedef std::unordered_map< const Symbol *, TextureSamplerPairSet > FunctionTextureSamplerMap;

        const ParseTree &           mParseTree;
        NameSymbolMap               mNameSymbolMap;

        ParseTreeMap                mVariables;
        FunctionTextureSamplerMap   mFunctionTextureSamplerMap;
    };

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_SHADER_BUILDER_H__
