#include "common/trace.h"

#include "gfw/base/buffer.h"
#include "gfw/base/context.h"
#include "gfw/base/input_layout.h"

#include "gfw/runtime/common/mesh.h"

#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    Mesh::Mesh(IDeviceIn device)
        : ADeviceChild(device)
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

        context->SetInputLayout(mInputLayout);

        for (uint32_t i = 0; i < mBufCnt; ++ i)
        {
            context->SetVertexBuffer(i, mVertexBuffers[i]);
        }

        context->SetIndexBuffer(mIndexBuffer);

        context->Draw(mDrawParams);
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
