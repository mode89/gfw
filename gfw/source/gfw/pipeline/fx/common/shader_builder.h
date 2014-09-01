#ifndef __GFW_PIPELINE_FX_COMMON_SHADER_BUILDER_H__
#define __GFW_PIPELINE_FX_COMMON_SHADER_BUILDER_H__

#include "gfw/pipeline/fx/common/symbol.h"
#include <memory>
#include <string>

namespace GFW {

    class ParseTree;
    class ShaderBinary;

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

#endif // __GFW_PIPELINE_FX_COMMON_SHADER_BUILDER_H__
