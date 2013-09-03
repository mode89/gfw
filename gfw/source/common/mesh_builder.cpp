#include "common/trace.h"

#include "gfw/base/buffer.h"
#include "gfw/base/device.h"

#include "gfw/common/mesh.h"
#include "gfw/common/mesh_builder.h"

namespace GFW {

    MeshBuilder::MeshBuilder()
        : mAttrCnt(0)
        , mBufCnt(0)
    {

    }

    MeshBuilder::~MeshBuilder()
    {

    }

    void MeshBuilder::SetVertexAttributes(uint32_t attrCnt, VertexAttribute attrs[])
    {
        for (uint32_t i = 0; i < attrCnt; ++ i)
        {
            mVertexAttributes[i] = attrs[i];
        }

        for (uint32_t i = attrCnt; i < mAttrCnt; ++ i)
        {
            mVertexAttributes[i] = VertexAttribute();
        }

        mAttrCnt = attrCnt;
    }

    void MeshBuilder::SetVertexBuffers(uint32_t bufCnt, IBufferRef bufs[])
    {
        for (uint32_t i = 0; i < bufCnt; ++ i)
        {
            mVertexBuffers[i] = bufs[i];
        }

        for (uint32_t i = bufCnt; i < mBufCnt; ++ i)
        {
            mVertexBuffers[i].Detach();
        }

        mBufCnt = bufCnt;
    }

    IMeshRef MeshBuilder::Build(IDeviceIn device)
    {
        Mesh * mesh = new Mesh(device);

        mesh->SetVertexAttributes(mAttrCnt, mVertexAttributes);
        mesh->SetVertexBuffers(mBufCnt, mVertexBuffers);
        mesh->SetIndexBuffer(mIndexBuffer);
        mesh->SetDrawParams(mDrawParams);

        return mesh;
    }

} // namespace GFW
