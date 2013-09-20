#ifndef __GFW_BASE_MESH_BUILDER_H__
#define __GFW_BASE_MESH_BUILDER_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    class IMeshBuilder : public Common::ARefCounted
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
    AUTOREF_REFERENCE_DECLARATION(IMeshBuilder);

} // namespace GFW

#endif // __GFW_BASE_MESH_BUILDER_H__
