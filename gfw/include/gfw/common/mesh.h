#ifndef __GFW_COMMON_MESH_H__
#define __GFW_COMMON_MESH_H__

#include "gfw/base/draw_params.h"
#include "gfw/base/mesh.h"
#include "gfw/common/device_child.h"
#include "gfw/core/limits.h"

namespace GFW {

    class Mesh : public ADeviceChild<IMesh>
    {
    public:
        virtual void
        Draw();

    public:
        void inline
        SetInputLayout(IInputLayoutIn il) { mInputLayout = il; }

        void
        SetVertexBuffers(uint32_t bufCnt, IBufferRef[]);

        void inline
        SetIndexBuffer(IBufferIn buffer) { mIndexBuffer = buffer; }

        void inline
        SetDrawParams(const DrawIndexedParams & params) { mDrawParams = params; }

    public:
        Mesh(IDeviceIn);
        ~Mesh();

    private:
        IInputLayoutRef     mInputLayout;

        IBufferRef          mVertexBuffers[MAX_BIND_VERTEX_BUFFERS];
        uint32_t            mBufCnt;

        IBufferRef          mIndexBuffer;

        DrawIndexedParams   mDrawParams;
    };
    AUTOREF_REFERENCE_DECLARATION(Mesh);

} // namespace GFW

#endif // __GFW_COMMON_MESH_H__
