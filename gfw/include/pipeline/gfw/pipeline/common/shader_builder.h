#ifndef __GFW_PIPELINE_COMMON_SHADER_BUILDER_H__
#define __GFW_PIPELINE_COMMON_SHADER_BUILDER_H__

#include "gfw/shared/shader.h"

namespace GFW {

    class IShaderBuilder : public Common::ARefCounted
    {
    public:
        virtual ShaderBinaryRef
        Compile( const char * entryName, const char * profile ) = 0;

        virtual
        ~IShaderBuilder() {}
    };
    AUTOREF_REFERENCE_DECLARATION( IShaderBuilder );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SHADER_BUILDER_H__
