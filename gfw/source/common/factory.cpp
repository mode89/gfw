#include "common/trace.h"

#include "gfw/base/buffer.h"
#include "gfw/base/device.h"
#include "gfw/base/draw_params.h"
#include "gfw/base/vertex_attribute.h"

#include "gfw/common/effect.h"
#include "gfw/common/factory.h"
#include "gfw/common/mesh.h"
#include "gfw/common/shader_stage.h"

#include <fstream>
#include <string>

namespace GFW {

    using namespace Common;

    IFactoryRef CreateFactory(IDeviceRef device)
    {
        Factory * factory = new Factory(device);
        if (!factory->Initialize())
        {
            TRACE_FAIL_MSG("Failed to initialize a factory");
            delete factory;
            return NULL;
        }
        return factory;
    }

    Factory::Factory(IDeviceRef device)
        : mDevice(device)
    {

    }

    Factory::~Factory()
    {

    }

    bool Factory::Initialize()
    {
        return true;
    }

    IEffectRef Factory::CreateEffect(const char * fileName)
    {
        TRACE_ASSERT(fileName != NULL);

        std::ifstream file(fileName, std::ios_base::binary);
        TRACE_ASSERT(file);

        // Determine size

        file.seekg(0, file.end);
        uint64_t fileSize = file.tellg();
        file.seekg(0, file.beg);

        // Read the effect file

        Common::AutoPointer<char> effectData = new char [static_cast<uint32_t>(fileSize + 1)];
        file.read(effectData, fileSize);
        effectData[fileSize] = 0;

        // Create shaders

        IShaderRef shaders[SHADER_STAGE_COUNT];

        for (int stage = 0; stage < SHADER_STAGE_COUNT; ++ stage)
        {
            std::string shaderData = "#define GFW_SHADER_STAGE_";
            shaderData += GetStageString(static_cast<ShaderStage>(stage));
            shaderData += "\n";
            shaderData += effectData;

            shaders[stage] = mDevice->CreateShader(static_cast<ShaderStage>(stage), shaderData.c_str());
            TRACE_ASSERT(shaders[stage].IsAttached());
        }

        EffectRef effect = new Effect(shaders, mDevice);
        return effect.StaticCast<IEffect>();
    }

    IMeshRef Factory::CreateSurfaceMesh(
        float xLeft,
        float yBottom,
        float xRight,
        float yTop,
        uint32_t xSegments,
        uint32_t ySegments)
    {
        struct Vertex
        {
            float x;
            float y;
            float z;
        };

        uint32_t xVertCnt = xSegments + 1;
        uint32_t yVertCnt = ySegments + 1;
        uint32_t vertCnt  = xVertCnt * yVertCnt;

        // Create vertex buffer

        AutoPointer<Vertex> vertices = new Vertex [vertCnt];
        for (uint32_t j = 0; j < yVertCnt; ++ j)
        {
            uint32_t offset = j * xVertCnt;
            for (uint32_t i = 0; i < xVertCnt; ++ i)
            {
                static const float xStep = (xRight - xLeft) / xSegments;
                static const float yStep = (yTop - yBottom) / ySegments;

                Vertex & vertex = vertices[i + offset];
                vertex.x = xLeft + xStep * i;
                vertex.y = yBottom + yStep * j;
                vertex.z = 0.0f;
            }
        }

        BufferDesc vertexBufferDesc;
        vertexBufferDesc.size  = sizeof(Vertex) * vertCnt;
        vertexBufferDesc.type  = BUFFER_VERTEX;
        vertexBufferDesc.usage = USAGE_STATIC;
        IBufferRef vertexBuffer = mDevice->CreateBuffer(vertexBufferDesc, vertices);

        // Create index buffer

        uint32_t indexCount = xSegments * ySegments * 2 * 3;
        AutoPointer<uint32_t> indices = new uint32_t [indexCount];
        for (uint32_t j = 0; j < ySegments; ++ j)
        {
            for (uint32_t i = 0; i < xSegments; ++ i)
            {
                uint32_t offset = (i + j * xSegments) * 2 * 3;
                for (uint32_t k = 0; k < 6; ++ k)
                {
                    static const uint32_t pattern[][2] = {
                        { 0, 1 },
                        { 1, 1 },
                        { 0, 0 },
                        { 1, 1 },
                        { 1, 0 },
                        { 0, 0 }
                    };

                    indices[offset + k] = (i + pattern[k][0]) + (j + pattern[k][1]) * xVertCnt;
                }
            }
        }

        BufferDesc indexBufferDesc;
        indexBufferDesc.size  = sizeof(uint32_t) * xSegments * ySegments * 2 * 3;
        indexBufferDesc.type  = BUFFER_INDEX;
        indexBufferDesc.usage = USAGE_STATIC;
        IBufferRef indexBuffer = mDevice->CreateBuffer(indexBufferDesc, indices);

        // Vertex attributes

        VertexAttribute vertexAttribute;
        vertexAttribute.semantic = SEMANTIC_POSITION0;
        vertexAttribute.format   = FORMAT_R32G32B32_FLOAT;
        vertexAttribute.stride   = sizeof(Vertex);

        // Drawing parameters

        DrawIndexedParams drawParams;
        drawParams.primTop    = PRIM_TRIANGLES;
        drawParams.indexType  = TYPE_UNSIGNED_INT;
        drawParams.indexStart = 0;
        drawParams.indexCount = indexCount;

        // Build mesh

        Mesh * mesh = new Mesh(mDevice);
        mesh->SetVertexBuffers(1, &vertexBuffer);
        mesh->SetIndexBuffer(indexBuffer);
        mesh->SetVertexAttributes(1, &vertexAttribute);
        mesh->SetDrawParams(drawParams);

        return mesh;
    }

} // namespace GFW
