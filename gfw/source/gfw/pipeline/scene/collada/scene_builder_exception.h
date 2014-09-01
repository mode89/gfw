#ifndef __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_EXCEPTION_H__
#define __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_EXCEPTION_H__

#include <stdexcept>

namespace GFW {

    #define EXCEPTION_NAMESPACE SceneBuilderException

    #define EXCEPTION_CODES \
        C( FileNotFound, "File not found." ) \
        C( XmlError, "Failed to read the file." ) \

    #include "cmn/exception.inl"

    #undef EXCEPTION_NAMESPACE
    #undef EXCEPTION_CODES

};

#endif // __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_EXCEPTION_H__
