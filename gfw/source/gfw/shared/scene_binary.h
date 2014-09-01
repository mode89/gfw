#ifndef __GFW_SHARED_SCENE_BINARY_H__
#define __GFW_SHARED_SCENE_BINARY_H__

#include "gfw/semantic.h"
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
                : semantic( 0 )
                , slot( 0 )
                , stride( 0 )
                , offset( 0 )
            {}
        };

        typedef std::vector< float > FloatBuffer;
        typedef std::vector< int > IntBuffer;
        typedef std::vector< FloatBuffer > FloatBufferVec;
        typedef std::vector< VertexAttribute > AttributeVec;

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
        typedef std::list< const GeometryBinary * > GeometryList;
        typedef std::list< NodeBinary > NodeList;

        float           matrix[ 16 ];
        GeometryList    geometries;
        NodeList        nodes;
    };

    struct SceneBinary
    {
        typedef std::list< GeometryBinary > GeometryTable;
        typedef std::list< NodeBinary > NodeList;

        GeometryTable   geometryTable;
        NodeList        nodes;
    };

} // namespace GFW

#endif // __GFW_SHARED_SCENE_BINARY_H__
