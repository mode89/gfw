#include "common/trace.h"

#include "gfw/base/buffer.h"
#include "gfw/base/context.h"

#include "gfw/common/mesh.h"

#include "gfw/common/device_child.inl"

namespace GFW {

    Mesh::Mesh(IDeviceIn device)
        : ADeviceChild(device)
        , mAttrCnt(0)
        , mBufCnt(0)
    {

    }

    Mesh::~Mesh()
    {

    }

    void Mesh::Draw()
    {
        IContextRef context = mDevice->GetCurrentContext();
        TRACE_ASSERT(context.IsAttached());

        context->SetVertexAttributes(mAttrCnt, mVertexAttributes);

        for (uint32_t i = 0; i < mBufCnt; ++ i)
        {
            context->SetVertexBuffer(i, mVertexBuffers[i]);
        }

        context->SetIndexBuffer(mIndexBuffer);

        context->Draw(mDrawParams);
    }

    void Mesh::SetVertexAttributes(uint32_t attrCnt, VertexAttribute attrs[])
    {
        for (uint32_t i = 0; i < attrCnt; ++ i)
        {
            TRACE_ASSERT(attrs[i].semantic != SEMANTIC_UNKNOWN);
            mVertexAttributes[i] = attrs[i];
        }

        for (uint32_t i = attrCnt; i < mAttrCnt; ++ i)
        {
            mVertexAttributes[i] = VertexAttribute();
        }

        mAttrCnt = attrCnt;
    }

    void Mesh::SetVertexBuffers(uint32_t bufCnt, IBufferRef bufs[])
    {
        for (uint32_t i = 0; i < bufCnt; ++ i)
        {
            TRACE_ASSERT(bufs[i].IsAttached());
            mVertexBuffers[i] = bufs[i];
        }

        for (uint32_t i = bufCnt; i < mBufCnt; ++ i)
        {
            mVertexBuffers[i].Detach();
        }

        mBufCnt = bufCnt;
    }

} // namespace GFW
