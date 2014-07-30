#ifndef __GFW_RUNTIME_COMMON_MESH_BUILDER_H__
#define __GFW_RUNTIME_COMMON_MESH_BUILDER_H__

#include "gfw/mesh_builder.h"
#include "gfw/runtime/ogl4/limits.h" // TODO platform dependency

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

        IBufferRef          mVertexBuffers[MAX_BIND_VERTEX_BUFFERS];
        uint32_t            mBufCnt;

        IBufferRef          mIndexBuffer;

        DrawIndexedParams   mDrawParams;
    };

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_MESH_BUILDER_H__
