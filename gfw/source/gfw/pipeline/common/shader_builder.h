#ifndef __GFW_PIPELINE_COMMON_SHADER_BUILDER_H__
#define __GFW_PIPELINE_COMMON_SHADER_BUILDER_H__

#include <memory>
#include <string>

namespace GFW {

    class ParseTree;
    class ShaderBinary;
    class SymbolTable;

    class IShaderBuilder
    {
    public:
        // TODO try move semantic - return ShaderBinary by rvalue reference
        virtual void
        Build( ShaderBinary &, const std::string & entryPoint, const std::string & profile ) = 0;

        virtual
        ~IShaderBuilder() {}
    };

    std::shared_ptr< IShaderBuilder > CreateShaderBuilder( const ParseTree &, const SymbolTable & );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SHADER_BUILDER_H__
