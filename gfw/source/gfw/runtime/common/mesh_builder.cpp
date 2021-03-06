#include "cmn/trace.h"
#include "gfw/buffer.h"
#include "gfw/device.h"
#include "gfw/input_layout.h"
#include "gfw/runtime/common/mesh.h"
#include "gfw/runtime/common/mesh_builder.h"
#include "gfw/runtime/ogl4/device.h"

namespace GFW {

    MeshBuilder::MeshBuilder()
        : mBufCnt(0)
    {

    }

    MeshBuilder::~MeshBuilder()
    {

    }

    void MeshBuilder::SetVertexBuffers(uint32_t bufCnt, IBufferRef bufs[])
    {
        for (uint32_t i = 0; i < bufCnt; ++ i)
        {
            mVertexBuffers[i] = bufs[i];
        }

        for (uint32_t i = bufCnt; i < mBufCnt; ++ i)
        {
            mVertexBuffers[i].reset();
        }

        mBufCnt = bufCnt;
    }

    IMeshRef MeshBuilder::Build(IDeviceIn device)
    {
        MeshRef mesh = std::make_shared<Mesh>( device );

        mesh->SetInputLayout(mInputLayout);
        mesh->SetVertexBuffers(mBufCnt, mVertexBuffers);
        mesh->SetIndexBuffer(mIndexBuffer);
        mesh->SetDrawParams(mDrawParams);

        return mesh;
    }

} // namespace GFW
