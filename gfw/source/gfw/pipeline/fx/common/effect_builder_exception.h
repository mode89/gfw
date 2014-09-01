#ifndef __GFW_PIPELINE_COMMON_EFFECT_BUILDER_EXCEPTION_H__
#define __GFW_PIPELINE_COMMON_EFFECT_BUILDER_EXCEPTION_H__

#include <stdexcept>

namespace GFW {

    #define EXCEPTION_NAMESPACE EffectBuilderException

    #define EXCEPTION_CODES \
        C( EmptyCbuffer, "Cbuffer '%s' has no members." ) \
        C( ParsingError, "Failed to build a parsing tree." ) \
        C( ShaderBuilderError, "Failed to build the shader '%s'." ) \
        C( WrongRegister, "Wrong register specification of '%s'." ) \

    #include "cmn/exception.inl"

    #undef EXCEPTION_NAMESPACE
    #undef EXCEPTION_CODES

};

#endif // __GFW_PIPELINE_COMMON_EFFECT_BUILDER_EXCEPTION_H__
