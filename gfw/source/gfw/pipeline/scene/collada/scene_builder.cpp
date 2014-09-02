#include "cmn/trace.h"
#include "gfw/pipeline/scene/collada/scene_builder.h"
#include "gfw/pipeline/scene/collada/scene_builder_exception.h"
#include "gfw/shared/scene_binary.h"
#include "tinyxml2.h"
#include <cstring>
#include <unordered_map>

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

        XMLElement * xmlCollada = xmlDocument.FirstChildElement( "COLLADA" );
        CMN_THROW_IF( xmlCollada == nullptr,
            SceneBuilderException::BadFormat( "Failed to find COLLADA root." ) );

        // Build geometry table

            XMLElement * xmlLibraryGeometries = xmlCollada->FirstChildElement( "library_geometries" );
            if ( xmlLibraryGeometries )
            {
                //auto & geometryTable = sceneBinary.geometryTable;

                for ( XMLElement * xmlGeometry = xmlLibraryGeometries->FirstChildElement( "geometry" );
                    xmlGeometry != nullptr; xmlGeometry = xmlGeometry->NextSiblingElement( "geometry" ) )
                {
                    //geometryTable.emplace_back();
                    //GeometryBinary & geometryBinary = geometryTable.back();

                    for ( XMLElement * xmlGeometryType = xmlGeometry->FirstChildElement();
                        xmlGeometryType != nullptr; xmlGeometryType = xmlGeometryType->NextSiblingElement() )
                    {
                        if ( std::strcmp( xmlGeometryType->Name(), "mesh" ) == 0 )
                        {
                            //MeshBinary & meshBinary = geometryBinary.mesh;

                            // Collect sources
                            std::unordered_map< std::string, XMLElement * > sourceMap;
                            for ( XMLElement * xmlSource = xmlGeometryType->FirstChildElement( "source" );
                                xmlSource != nullptr; xmlSource = xmlSource->NextSiblingElement( "source" ) )
                            {
                                sourceMap[ xmlSource->Attribute( "id" ) ] = xmlSource;
                            }

                            // Collect unshared inputs
                            XMLElement * xmlVertices = xmlGeometryType->FirstChildElement( "vertices" );
                            for ( XMLElement * xmlInput = xmlVertices->FirstChildElement( "input" );
                                xmlInput != nullptr; xmlInput = xmlInput->NextSiblingElement( "input" ) )
                            {
                                //const char * semantic = xmlInput->Attribute( "semantic" );
                                //const char * sourceUri = xmlInput->Attribute( "source" ) + 1;

                                //XMLElement * xmlSource = sourceMap[ sourceUri ];
                            }

                            XMLElement * xmlTriangles = xmlGeometryType->FirstChildElement( "triangles" );
                            CMN_THROW_IF( xmlTriangles == nullptr,
                                SceneBuilderException::Unsupported( "Mesh has to be triangulated." ) );
                            CMN_THROW_IF( xmlTriangles->NextSiblingElement( "triangles" ) != nullptr,
                                SceneBuilderException::Unsupported( "Mesh can have only one set of triangles." ) );
                        }
                    }
                }
            }
    }

} // namespace GFW
