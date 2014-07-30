#ifndef __GFW_BASE_MESH_BUILDER_H__
#define __GFW_BASE_MESH_BUILDER_H__

#include "gfw/types_fwd.h"

namespace GFW {

    class IMeshBuilder
    {
    public:
        virtual void
        SetInputLayout(IInputLayoutIn) = 0;

        virtual void
        SetVertexBuffers(uint32_t bufCnt, IBufferRef[]) = 0;

        virtual void
        SetIndexBuffer(IBufferIn) = 0;

        virtual void
        SetDrawParams(const DrawIndexedParams &) = 0;

        virtual IMeshRef
        Build(IDeviceIn) = 0;

        virtual
        ~IMeshBuilder() {}
    };
    SHARED_PTR_TYPEDEFS(IMeshBuilder);

} // namespace GFW

#endif // __GFW_BASE_MESH_BUILDER_H__
