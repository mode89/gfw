#ifndef __GFW_SHARED_SCENE_BINARY_H__
#define __GFW_SHARED_SCENE_BINARY_H__

#include <list>
#include <vector>

namespace GFW {

    struct MeshBinary
    {
        struct VertexAttribute
        {
            uint32_t    semantic;
            uint32_t    slot;
            uint32_t    stride;
            uint32_t    offset;

            VertexAttribute()
                : semantic( SEMANTIC_UNKNOWN )
                , slot( 0 )
                , stride( 0 )
                , offset( 0 )
            {}
        };

        typedef std::vector< float > FloatBuffer;
        typedef std::vector< int > IntBuffer;
        typedef std::vector< FloatBuffer > FloatBufferVec;
        typedef std::vector< VectorAttribute > AttributeVec;

        AttributeVec    attributes;
        FloatBufferVec  vertexBuffers;
        IntBuffer       indexBuffer;
    };

    struct GeometryBinary
    {
        MeshBinary  mesh;
    };

    struct NodeBinary
    {
        std::list< const GeometryBinary * > GeometryList;
        std::list< NodeBinary > NodeList;

        float           matrix[ 16 ];
        GeometryList    geometries;
        NodeList        nodes;
    };

    struct SceneBinary
    {
        std::list< GeometryBinary > GeometryTable;
        std::list< NodeBinary > NodeList;

        GeometryList    geometryTable;
        NodeList        nodes;
    };

} // namespace GFW

#endif // __GFW_SHARED_SCENE_BINARY_H__
