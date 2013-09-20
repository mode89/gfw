#ifndef __GFW_COMMON_MESH_BUILDER_H__
#define __GFW_COMMON_MESH_BUILDER_H__

#include "gfw/base/mesh_builder.h"
#include "gfw/core/limits.h"

namespace GFW {

    class MeshBuilder : public IMeshBuilder
    {
    public:
        virtual void
        SetInputLayout(IInputLayoutIn il) { mInputLayout = il; }

        virtual void
        SetVertexBuffers(uint32_t bufCnt, IBufferRef[]);

        virtual void
        SetIndexBuffer(IBufferIn buffer) { mIndexBuffer = buffer; }

        virtual void
        SetDrawParams(const DrawIndexedParams & params) { mDrawParams = params; }

        virtual IMeshRef
        Build(IDeviceIn);

    public:
        MeshBuilder();
        ~MeshBuilder();

    private:
        IInputLayoutRef     mInputLayout;

        IBufferRef          mVertexBuffers[MAX_VERTEX_BUFFER_BIND];
        uint32_t            mBufCnt;

        IBufferRef          mIndexBuffer;

        DrawIndexedParams   mDrawParams;
    };

} // namespace GFW

#endif // __GFW_COMMON_MESH_BUILDER_H__
