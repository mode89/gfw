#ifndef __GFW_RUNTIME_COMMON_MESH_H__
#define __GFW_RUNTIME_COMMON_MESH_H__

#include "gfw/draw_params.h"
#include "gfw/mesh.h"
#include GFW_PLATFORM_LIMITS_H

namespace GFW {

    class Mesh : public IMesh
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
        Mesh( IDeviceIn );
        ~Mesh();

    private:
        IDeviceRef          mDevice;

        IInputLayoutRef     mInputLayout;

        IBufferRef          mVertexBuffers[MAX_BIND_VERTEX_BUFFERS];
        uint32_t            mBufCnt;

        IBufferRef          mIndexBuffer;

        DrawIndexedParams   mDrawParams;
    };
    SHARED_PTR_TYPEDEFS(Mesh);

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_MESH_H__
