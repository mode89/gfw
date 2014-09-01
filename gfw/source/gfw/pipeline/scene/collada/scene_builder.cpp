#include "cmn/trace.h"
#include "gfw/pipeline/scene/collada/scene_builder.h"
#include "gfw/pipeline/scene/collada/scene_builder_exception.h"
#include "tinyxml2.h"

using namespace tinyxml2;

namespace GFW {

    ISceneBuilderRef CreateSceneBuilderCollada()
    {
        return std::make_shared< SceneBuilderCollada >();
    }

    SceneBuilderCollada::SceneBuilderCollada()
    {
    }

    SceneBuilderCollada::~SceneBuilderCollada()
    {
    }

    void SceneBuilderCollada::Build( SceneBinary & sceneBinary, const boost::filesystem::path & filePath )
    {
        // Parsing XML
        XMLDocument xmlDocument;
        XMLError xmlError = xmlDocument.LoadFile( filePath.string().c_str() );
        CMN_THROW_IF( xmlError == XML_ERROR_FILE_NOT_FOUND, SceneBuilderException::FileNotFound() );
        CMN_THROW_IF( xmlError, SceneBuilderException::XmlError() );
    }

} // namespace GFW
