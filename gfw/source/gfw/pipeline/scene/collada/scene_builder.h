#ifndef __GFW_PIPELINE_SCENE_COLLADA_SCENE_BUILDER_H__
#define __GFW_PIPELINE_SCENE_COLLADA_SCENE_BUILDER_H__

#include "gfw/pipeline/scene/common/scene_builder.h"

namespace GFW {

    class SceneBuilderCollada : public ISceneBuilder
    {
    public:
        SceneBuilderCollada();
        ~SceneBuilderCollada();
    };

} // namespace GFW

#endif // __GFW_PIPELINE_SCENE_COLLADA_SCENE_BUILDER_H__
