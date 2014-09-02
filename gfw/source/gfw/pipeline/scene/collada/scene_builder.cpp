#include "cmn/trace.h"
#include "gfw/pipeline/scene/collada/scene_builder.h"
#include "gfw/pipeline/scene/collada/scene_builder_exception.h"
#include "gfw/shared/scene_binary.h"
#include "tinyxml2.h"
#include <cstring>
#include <sstream>
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
                auto & geometryTable = sceneBinary.geometryTable;

                for ( XMLElement * xmlGeometry = xmlLibraryGeometries->FirstChildElement( "geometry" );
                    xmlGeometry != nullptr; xmlGeometry = xmlGeometry->NextSiblingElement( "geometry" ) )
                {
                    geometryTable.emplace_back();
                    GeometryBinary & geometryBinary = geometryTable.back();

                    for ( XMLElement * xmlGeometryType = xmlGeometry->FirstChildElement();
                        xmlGeometryType != nullptr; xmlGeometryType = xmlGeometryType->NextSiblingElement() )
                    {
                        if ( std::strcmp( xmlGeometryType->Name(), "mesh" ) == 0 )
                        {
                            MeshBinary & meshBinary = geometryBinary.mesh;

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
                                const char * semanticString = xmlInput->Attribute( "semantic" );
                                unsigned semanticIndex = xmlInput->IntAttribute( "set" );
                                const char * sourceUri = xmlInput->Attribute( "source" ) + 1;

                                // Construct attribute

                                    meshBinary.attributes.emplace_back();
                                    auto & attribute = meshBinary.attributes.back();
                                    attribute.semantic = GetSemantic( semanticString, semanticIndex );
                                    attribute.slot = meshBinary.vertexBuffers.size();

                                    XMLElement * xmlSource = sourceMap[ sourceUri ];
                                    XMLElement * xmlTechniqueCommon = xmlSource->FirstChildElement( "technique_common" );
                                    XMLElement * xmlAccessor = xmlTechniqueCommon->FirstChildElement( "accessor" );
                                    attribute.stride = xmlAccessor->IntAttribute( "stride" );

                                // Construct buffer binary

                                    meshBinary.vertexBuffers.emplace_back();
                                    auto & buffer = meshBinary.vertexBuffers.back();
                                    XMLElement * xmlFloatArray = xmlSource->FirstChildElement( "float_array" );
                                    std::istringstream stream( xmlFloatArray->GetText() );
                                    std::copy(
                                        std::istream_iterator< float >( stream ),
                                        std::istream_iterator< float >(),
                                        std::back_inserter( buffer ) );
                            }

                            XMLElement * xmlTriangles = xmlGeometryType->FirstChildElement( "triangles" );
                            CMN_THROW_IF( xmlTriangles == nullptr,
                                SceneBuilderException::Unsupported( "Mesh has to be triangulated." ) );
                            CMN_THROW_IF( xmlTriangles->NextSiblingElement( "triangles" ) != nullptr,
                                SceneBuilderException::Unsupported( "Mesh can have only one set of triangles." ) );

                            XMLElement * xmlIndices = xmlTriangles->FirstChildElement( "p" );
                            std::vector< unsigned > indices;
                            std::istringstream indicesStream( xmlIndices->GetText() );
                            std::copy(
                                std::istream_iterator< unsigned >( indicesStream ),
                                std::istream_iterator< unsigned >(),
                                std::back_inserter( indices ) );

                            unsigned trianglesCount = xmlTriangles->IntAttribute( "count" );
                            unsigned vertexCount = trianglesCount * 3;
                            unsigned indexCountPerVertex = indices.size() / vertexCount;

                            // Collect shared inputs

                                for ( XMLElement * xmlInput = xmlTriangles->FirstChildElement( "input" );
                                    xmlInput != nullptr; xmlInput = xmlInput->NextSiblingElement( "input" ) )
                                {
                                    const char * semanticString = xmlInput->Attribute( "semantic" );
                                    if ( std::strcmp( semanticString, "VERTEX" ) != 0 )
                                    {
                                        unsigned semanticIndex = xmlInput->IntAttribute( "set" );
                                        const char * sourceUri = xmlInput->Attribute( "source" ) + 1;

                                        // Construct attribute

                                            meshBinary.attributes.emplace_back();
                                            auto & attribute = meshBinary.attributes.back();
                                            attribute.semantic = GetSemantic( semanticString, semanticIndex );
                                            attribute.slot = meshBinary.vertexBuffers.size();

                                            XMLElement * xmlSource = sourceMap[ sourceUri ];
                                            XMLElement * xmlTechniqueCommon = xmlSource->FirstChildElement( "technique_common" );
                                            XMLElement * xmlAccessor = xmlTechniqueCommon->FirstChildElement( "accessor" );
                                            attribute.stride = xmlAccessor->IntAttribute( "stride" );

                                        // Construct buffer binary

                                            std::vector< float > sourceBuffer;
                                            XMLElement * xmlFloatArray = xmlSource->FirstChildElement( "float_array" );
                                            std::istringstream stream( xmlFloatArray->GetText() );
                                            std::copy(
                                                std::istream_iterator< float >( stream ),
                                                std::istream_iterator< float >(),
                                                std::back_inserter( sourceBuffer ) );

                                            unsigned indexOffset = xmlInput->IntAttribute( "offset" );

                                            meshBinary.vertexBuffers.emplace_back();
                                            auto & buffer = meshBinary.vertexBuffers.back();
                                            buffer.reserve( vertexCount * attribute.stride );
                                            for ( unsigned i = indexOffset, n = indices.size(); i < n; i += indexCountPerVertex )
                                            {
                                                unsigned index = indices[ i ];
                                                unsigned offset = index * attribute.stride;
                                                for ( unsigned j = 0; j < attribute.stride; ++ j )
                                                {
                                                    buffer.push_back( sourceBuffer[ offset + j ] );
                                                }
                                            }
                                    }
                                }

                            // Construct index buffer

                                for ( XMLElement * xmlInput = xmlTriangles->FirstChildElement( "input" );
                                    xmlInput != nullptr; xmlInput = xmlInput->NextSiblingElement( "input" ) )
                                {
                                    const char * semanticString = xmlInput->Attribute( "semantic" );
                                    if ( std::strcmp( semanticString, "VERTEX" ) == 0 )
                                    {
                                        unsigned indexOffset = xmlInput->IntAttribute( "offset" );
                                        auto & buffer = meshBinary.indexBuffer;
                                        buffer.reserve( vertexCount );
                                        for ( unsigned i = indexOffset, n = indices.size(); i < n; i += indexCountPerVertex )
                                        {
                                            buffer.push_back( indices[ i ] );
                                        }
                                        break;
                                    }
                                }
                        }
                    }
                }
            }
    }

} // namespace GFW
