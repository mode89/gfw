#ifndef __GFW_COMMON_MESH_H__
#define __GFW_COMMON_MESH_H__

#include "gfw/base/draw_params.h"
#include "gfw/base/mesh.h"
#include "gfw/base/vertex_attribute.h"
#include "gfw/common/device_child.h"
#include "gfw/core/limits.h"

namespace GFW {

    class Mesh : public ADeviceChild<IMesh>
    {
    public:
        virtual void
        Draw();

    public:
        void
        SetVertexAttributes(uint32_t attrCnt, VertexAttribute[]);

        void
        SetVertexBuffers(uint32_t bufCnt, IBufferRef[]);

        void inline
        SetIndexBuffer(IBufferIn buffer) { mIndexBuffer = buffer; }

        void inline
        SetDrawParams(DrawIndexedParams params) { mDrawParams = params; }

    public:
        Mesh(IDeviceRef);
        ~Mesh();

    private:
        VertexAttribute     mVertexAttributes[MAX_VERTEX_BUFFER_BIND];
        uint32_t            mAttrCnt;

        IBufferRef          mVertexBuffers[MAX_VERTEX_BUFFER_BIND];
        uint32_t            mBufCnt;

        IBufferRef          mIndexBuffer;

        DrawIndexedParams   mDrawParams;
    };
    AUTOREF_REFERENCE_DECLARATION(Mesh);

} // namespace GFW

#endif // __GFW_COMMON_MESH_H__
