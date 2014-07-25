#include "cmn/trace.h"
#include "gfw/base/buffer.h"
#include "gfw/base/context.h"
#include "gfw/base/device.h"
#include "gfw/base/input_layout.h"
#include "gfw/runtime/common/mesh.h"

namespace GFW {

    Mesh::Mesh( IDeviceIn device )
        : mDevice( device )
        , mBufCnt( 0 )
    {

    }

    Mesh::~Mesh()
    {

    }

    void Mesh::Draw()
    {
        IContextRef context = mDevice->GetCurrentContext();
        CMN_ASSERT( context );

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
            CMN_ASSERT( bufs[i] );
            mVertexBuffers[i] = bufs[i];
        }

        for (uint32_t i = bufCnt; i < mBufCnt; ++ i)
        {
            mVertexBuffers[i].reset();
        }

        mBufCnt = bufCnt;
    }

} // namespace GFW
