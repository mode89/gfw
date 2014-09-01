#ifndef __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_H__
#define __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_H__

#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4548 4625 4626 4640 )
#include "boost/filesystem.hpp"
CMN_WARNING_POP

#include "cmn/shared_ptr_typedefs.h"
#include <memory>

namespace GFW {

    struct SceneBinary;

    class ISceneBuilder
    {
    public:
        virtual void
        Build( SceneBinary &, const boost::filesystem::path & ) = 0;

        virtual
        ~ISceneBuilder() {}
    };
    SHARED_PTR_TYPEDEFS( ISceneBuilder );

    ISceneBuilderRef
    CreateSceneBuilderCollada();

} // namespace GFW

#endif // __GFW_PIPELINE_SCENE_COMMON_SCENE_BUILDER_H__
