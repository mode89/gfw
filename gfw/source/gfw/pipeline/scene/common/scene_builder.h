#ifndef __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_H__
#define __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_H__

#include "cmn/shared_ptr_typedefs.h"
#include <memory>

namespace GFW {

    class ISceneBuilder
    {
    public:
        virtual
        ~ISceneBuilder() {}
    };
    SHARED_PTR_TYPEDEFS( ISceneBuilder );

    ISceneBuilderRef
    CreateSceneBuilderCollada();

} // namespace GFW

#endif // __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_H__
