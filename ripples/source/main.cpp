#include "common/trace.h"
#include "common/file.h"

#include "gfw/gfw.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

using namespace Common;
using namespace GFW;

static const uint32_t kWindowWidth  = 800;
static const uint32_t kWindowHeight = 600;

static const uint32_t kMaxMeshNodeCount = 1000;

struct Vertex
{
    float x;
    float y;
    float z;
};

int main()
{
    // Initialize the mesh

    uint32_t xNodeDim = static_cast<uint32_t>(sqrtf(kMaxMeshNodeCount * kWindowWidth / kWindowHeight));
    uint32_t yNodeDim = kMaxMeshNodeCount / xNodeDim;
    uint32_t nodeCnt  = xNodeDim * yNodeDim;

    TRACE_MESSAGE_FORMATTED("Node count: %d\n", nodeCnt);

    // Initialize graphics context

    WindowDesc windowDesc;
    windowDesc.width  = kWindowWidth;
    windowDesc.height = kWindowHeight;
    WindowHandle window = CreateDefaultWindow(windowDesc);

    DeviceParams deviceParams;
    deviceParams.width          = kWindowWidth;
    deviceParams.height         = kWindowHeight;
    deviceParams.windowHandle   = window;

    IDeviceRef device = CreateDevice(deviceParams);

    IContextRef context = device->GetDefaultContext();

    // Initialize geometry

    AutoPointer<Vertex> u = new Vertex [nodeCnt];
    AutoPointer<float> du_dt = new float [nodeCnt];
    for (uint32_t j = 0; j < yNodeDim; ++ j)
    {
        uint32_t offset = j * xNodeDim;
        for (uint32_t i = 0; i < xNodeDim; ++ i)
        {
            static const float xStep = 2.0f / (xNodeDim - 1);
            static const float yStep = 2.0f / (yNodeDim - 1);

            Vertex & vertex = u[i + offset];
            vertex.x = -1.0f + xStep * i;
            vertex.y = -1.0f + yStep * j;
            vertex.z = 1.0f * rand() / RAND_MAX - 0.5f;

            du_dt[i + offset] = 0.0f;
        }
    }

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.size  = sizeof(Vertex) * nodeCnt;
    vertexBufferDesc.type  = BUFFER_VERTEX;
    vertexBufferDesc.usage = USAGE_DYNAMIC;
    IBufferRef vertexBuffer = device->CreateBuffer(vertexBufferDesc, u);

    uint32_t indexCount = (xNodeDim - 1) * (yNodeDim - 1) * 2 * 3;
    AutoPointer<uint16_t> indexBufferData = new uint16_t [indexCount];
    for (uint32_t j = 0; j < (yNodeDim - 1); ++ j)
    {
        for (uint32_t i = 0; i < (xNodeDim - 1); ++ i)
        {
            uint32_t offset = (i + j * (xNodeDim - 1)) * 2 * 3;
            for (uint32_t k = 0; k < 6; ++ k)
            {
                static const uint32_t pattern[][2] = {
                    { 0, 1 },
                    { 0, 0 },
                    { 1, 1 },
                    { 1, 1 },
                    { 0, 0 },
                    { 1, 0 }
                };

                indexBufferData[offset + k] = (i + pattern[k][0]) + (j + pattern[k][1]) * xNodeDim;
            }
        }
    }

    BufferDesc indexBufferDesc;
    indexBufferDesc.size  = 2 * (xNodeDim - 1) * (yNodeDim - 1) * 2 * 3;
    indexBufferDesc.type  = BUFFER_INDEX;
    indexBufferDesc.usage = USAGE_STATIC;
    IBufferRef indexBuffer = device->CreateBuffer(indexBufferDesc, indexBufferData);
    indexBufferData.Detach();

    VertexAttribute vertexAttributes[] = {
        { "vs_vPosition", FORMAT_R32G32B32_FLOAT, sizeof(Vertex), 0, 0 }
    };

    // Initialize pipeline

    FileRef vertexShaderFile = new File();
    vertexShaderFile->Read("draw.vs.glsl");
    IShaderRef vertexShader = device->CreateShader(SHADER_STAGE_VERTEX, vertexShaderFile->GetData());

    FileRef pixelShaderFile = new File();
    pixelShaderFile->Read("draw.ps.glsl");
    IShaderRef pixelShader  = device->CreateShader(SHADER_STAGE_PIXEL, pixelShaderFile->GetData());

    // Drawing parameters

    ClearParams clearParams;
    clearParams.mask  = CLEAR_COLOR;
    clearParams.color[0] = 0.0f;
    clearParams.color[1] = 0.0f;
    clearParams.color[2] = 0.3f;
    clearParams.color[3] = 1.0f;

    DrawIndexedParams drawParams;
    drawParams.primTop    = PRIM_TRIANGLES;
    drawParams.indexType  = TYPE_UNSIGNED_SHORT;
    drawParams.indexStart = 0;
    drawParams.indexCount = indexCount;

    while (ProcessDefaultWindow(window))
    {
        // Calculate the mesh

        for (uint32_t j = 0; j < yNodeDim; ++ j)
        {
            uint32_t offset = j * xNodeDim;
            for (uint32_t i = 0; i < xNodeDim; ++ i)
            {
                static const float xStep = 1.0f / (xNodeDim - 1);
                static const float yStep = 1.0f / (yNodeDim - 1) * kWindowHeight / kWindowWidth;
                static const float tStep = 0.01f;

                float d2u_dx2 =
                    ((i < xNodeDim - 1) ? u[offset + i + 1].z : 0.0f) +
                    ((i > 0) ? u[offset + i - 1].z : 0.0f) -
                    2 * u[offset + i].z;

                float d2u_dy2 =
                    ((j < yNodeDim - 1) ? u[offset + i + xNodeDim].z : 0.0f) +
                    ((j > 0) ? u[offset + i - xNodeDim].z : 0.0f) -
                    2 * u[offset + i].z;

                u[offset + i].z += du_dt[offset + i] * tStep;
                du_dt[offset + i] += (d2u_dx2 + d2u_dy2 - du_dt[offset + i] * 0.1f) * tStep;
            }
        }

        context->BeginScene();
        {
            // Update the geometry

            vertexBuffer->UpdateSubresource(u);

            // Draw the geometry

            context->Clear(clearParams);

            context->SetVertexAttributes(1, vertexAttributes);
            context->SetVertexBuffer(0, vertexBuffer);
            context->SetIndexBuffer(indexBuffer);

            context->SetShader(SHADER_STAGE_VERTEX, vertexShader);
            context->SetShader(SHADER_STAGE_PIXEL,  pixelShader);

            context->Draw(drawParams);
        }
        context->EndScene();

        device->Present();
    }

    DestroyDefaultWindow(window);

    return 0;
}
