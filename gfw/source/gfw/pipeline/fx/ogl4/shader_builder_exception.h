#ifndef __GFW_PIPELINE_FX_CORE_SHADER_BUILDER_EXCEPTION_H__
#define __GFW_PIPELINE_FX_CORE_SHADER_BUILDER_EXCEPTION_H__

namespace GFW {

    #define EXCEPTION_NAMESPACE ShaderBuilderException

    #define EXCEPTION_CODES \
        C( AssignedSameRegister, "The same register cannot be assigned to multiple variables: '%s', '%s'." ) \
        C( ExceededCbufferRegistersLimit, "Exceeded cbuffer registers limit." ) \
        C( ExceededSamplerRegistersLimit, "Exceeded smapler registers limit." ) \
        C( ExceededTextureRegistersLimit, "Exceeded texture registers limit." ) \
        C( ParameterWithoutSemantic, "Input/output parameter wihtout semantic is not allowed." ) \
        C( UndefinedEntry, "Failed to find shader entry point '%s'." ) \
        C( UndefinedTextureObject, "Failed to find texture object '%s' in the global scope." ) \
        C( UndefinedSamplerObject, "Failed to find sampler object '%s' in the global scope." ) \
        C( ValidationError, "Failed to validate the shader." ) \

    #include "cmn/exception.inl"

    #undef EXCEPTION_NAMESPACE
    #undef EXCEPTION_CODES

} // namespace GFW

#endif // __GFW_PIPELINE_FX_CORE_SHADER_BUILDER_EXCEPTION_H__
