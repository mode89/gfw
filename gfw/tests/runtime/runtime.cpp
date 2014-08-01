#include "gfw_tests.h"

using namespace GFW;

TEST_F(GfwTests, Clear)
{
    // Main loop

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            ConstIRenderTargetRef rt[] = { mDefaultRenderTarget };
            mContext->SetRenderTargets( 1, rt );
            mContext->Clear( mClearParams );
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F( GfwTests, DrawColored )
{
    // Create effect

    IEffectRef         effect    = mFactory->CreateEffect( mDevice, "draw.fxc" );
	ConstITechniqueRef technique = effect->GetTechnique( "DrawColoredVertices" );

    // Create geometry

    float vertices[] = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.type = BUFFER_VERTEX;
    vertexBufferDesc.size = sizeof( vertices );
    vertexBufferDesc.usage = USAGE_STATIC;
    IBufferRef vertexBuffer = mDevice->CreateBuffer(vertexBufferDesc, vertices);

    // Define vertex attributes

    VertexAttribute vertexAttribs[2];
    vertexAttribs[0].semantic = SEMANTIC_POSITION0;
    vertexAttribs[0].format   = FORMAT_RG32_FLOAT;
    vertexAttribs[0].stride   = 20;
    vertexAttribs[1].semantic = SEMANTIC_COLOR0;
    vertexAttribs[1].format   = FORMAT_RGB32_FLOAT;
    vertexAttribs[1].stride   = 20;
    vertexAttribs[1].offset   = 8;
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout( 2, vertexAttribs, technique->GetShader( ShaderStage::VERTEX ) );

    // Define draw params

    DrawParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES;
    drawParams.vertexStart = 0;
    drawParams.vertexCount = 3;

    // Main loop

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            ConstIRenderTargetRef rt[] = { mDefaultRenderTarget };
            mContext->SetRenderTargets( 1, rt );
            mContext->Clear( mClearParams );

            technique->Dispatch();

            mContext->SetInputLayout(inputLayout);
            mContext->SetVertexBuffer(0, vertexBuffer);

            mContext->Draw(drawParams);
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F(GfwTests, DrawIndexed)
{
    // Create effect

    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef technique = effect->GetTechnique( "DrawColoredVertices" );

    // Create geometry

    float vertices[] = {
        -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f
    };

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.type  = BUFFER_VERTEX;
    vertexBufferDesc.size  = sizeof(vertices);
    vertexBufferDesc.usage = USAGE_STATIC;
    IBufferRef vertexBuffer = mDevice->CreateBuffer(vertexBufferDesc, vertices);

    uint16_t indices[] = {
        3, 0, 2, 1
    };

    BufferDesc indexBufferDesc;
    indexBufferDesc.type  = BUFFER_INDEX;
    indexBufferDesc.size  = sizeof(indices);
    indexBufferDesc.usage = USAGE_STATIC;
    IBufferRef indexBuffer = mDevice->CreateBuffer(indexBufferDesc, indices);

    // Define vertex attributes

    VertexAttribute vertexAttribs[2];
    vertexAttribs[0].semantic = SEMANTIC_POSITION0;
    vertexAttribs[0].format   = FORMAT_RG32_FLOAT;
    vertexAttribs[0].stride   = 20;
    vertexAttribs[1].semantic = SEMANTIC_COLOR0;
    vertexAttribs[1].format   = FORMAT_RGB32_FLOAT;
    vertexAttribs[1].stride   = 20;
    vertexAttribs[1].offset   = 8;
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout(2, vertexAttribs, technique->GetShader(ShaderStage::VERTEX));

    // Define draw params

    DrawIndexedParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES_STRIP;
    drawParams.indexType   = TYPE_USHORT;
    drawParams.indexStart  = 0;
    drawParams.indexCount  = 4;

    // Main loop

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            ConstIRenderTargetRef rt[] = { mDefaultRenderTarget };
            mContext->SetRenderTargets( 1, rt );
            mContext->Clear( mClearParams );

            technique->Dispatch();

            mContext->SetInputLayout(inputLayout);
            mContext->SetVertexBuffer(0, vertexBuffer);
            mContext->SetIndexBuffer(indexBuffer);

            mContext->Draw(drawParams);
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F(GfwTests, DrawScreenQuad)
{
    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef tech = effect->GetTechnique( "DrawRedQuad" );

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            ConstIRenderTargetRef rt[] = { mDefaultRenderTarget };
            mContext->SetRenderTargets( 1, rt );
            mContext->Clear( mClearParams );

            effect->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F(GfwTests, Reflect)
{
    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
}

struct Vertex
{
    float x;
    float y;
    float z;
};

TEST_F(GfwTests, CreateMesh)
{
    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );

    float    xLeft     = -1.0f;
    float    yBottom   = -1.0f;
    float    xRight    = 1.0f;
    float    yTop      = 1.0f;
    uint32_t xSegments = 16;
    uint32_t ySegments = 12;

    uint32_t xVertCnt = xSegments + 1;
    uint32_t yVertCnt = ySegments + 1;
    uint32_t vertCnt  = xVertCnt * yVertCnt;

    // Create vertex buffer

    std::vector< Vertex > vertices( vertCnt );
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
    IBufferRef vertexBuffer = mDevice->CreateBuffer( vertexBufferDesc, vertices.data() );

    // Create index buffer

    uint32_t indexCount = xSegments * ySegments * 2 * 3;
    std::vector< uint32_t > indices( indexCount );
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
    IBufferRef indexBuffer = mDevice->CreateBuffer( indexBufferDesc, indices.data() );

    // Create input layout

    VertexAttribute vertexAttribute;
    vertexAttribute.semantic = SEMANTIC_POSITION0;
    vertexAttribute.format   = FORMAT_RGB32_FLOAT;
    vertexAttribute.stride   = sizeof(Vertex);
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout(1, &vertexAttribute, effect->GetShader(ShaderStage::VERTEX));

    // Drawing parameters

    DrawIndexedParams drawParams;
    drawParams.primTop    = PRIM_TRIANGLES;
    drawParams.indexType  = TYPE_UINT;
    drawParams.indexStart = 0;
    drawParams.indexCount = indexCount;

    IMeshBuilderRef meshBuilder = mFactory->CreateMeshBuilder();
    meshBuilder->SetInputLayout(inputLayout);
    meshBuilder->SetVertexBuffers(1, &vertexBuffer);
    meshBuilder->SetIndexBuffer(indexBuffer);
    meshBuilder->SetDrawParams(drawParams);
    IMeshRef mesh = meshBuilder->Build(mDevice);

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            ConstIRenderTargetRef rt[] = { mDefaultRenderTarget };
            mContext->SetRenderTargets( 1, rt );
            mContext->Clear( mClearParams );

            effect->Dispatch();
            mesh->Draw();
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F(GfwTests, MapBuffer)
{
    static const uint32_t kDataCount = 100;

    // Allocate system copy of the buffer data
    std::vector< uint32_t > data( kDataCount );

    uint32_t bufferSize = sizeof(uint32_t) * kDataCount;

    BufferDesc bufferDesc;
    bufferDesc.size  = bufferSize;
    bufferDesc.usage = USAGE_DYNAMIC;
    bufferDesc.type  = BUFFER_VERTEX;
    IBufferRef buffer = mDevice->CreateBuffer(bufferDesc);

    for (int i = 0; i < 60; ++ i)
    {
        // Prepare a random data

        for (uint32_t i = 0; i < kDataCount; ++ i)
        {
            data[i] = rand();
        }

        // Write the buffer

        mContext->BeginScene();
        {
            uint32_t * mappedData = static_cast< uint32_t * >( mContext->Map( buffer, SubResourceIndex(), MAP_TYPE_WRITE ) );
            memcpy( mappedData, data.data(), bufferSize );
            mContext->Unmap( buffer, SubResourceIndex() );
        }
        mContext->EndScene();

        // Compare buffer with the data

        mContext->BeginScene();
        {
            uint32_t * mappedData = static_cast< uint32_t * >( mContext->Map( buffer, SubResourceIndex(), MAP_TYPE_READ ) );
            ASSERT_TRUE( memcmp( mappedData, data.data(), bufferSize ) == 0 );
            mContext->Unmap( buffer, SubResourceIndex() );
        }
        mContext->EndScene();
    }
}

TEST_F(GfwTests, UpdateBuffer)
{
    static const uint32_t kDataCount = 100;

    // Allocate system copy of the buffer data
    std::vector< uint32_t > data( kDataCount );

    uint32_t bufferSize = sizeof(uint32_t) * kDataCount;

    BufferDesc bufferDesc;
    bufferDesc.size  = bufferSize;
    bufferDesc.usage = USAGE_DYNAMIC;
    bufferDesc.type  = BUFFER_VERTEX;
    IBufferRef buffer = mDevice->CreateBuffer(bufferDesc);

    for (int i = 0; i < 60; ++ i)
    {
        // Prepare a random data

        for ( uint32_t i = 0; i < kDataCount; ++ i )
        {
            data[i] = rand();
        }

        // Write the buffer

        mContext->BeginScene();
        mContext->UpdateSubresource( buffer, SubResourceIndex(), data.data() );
        mContext->EndScene();

        // Compare buffer with the data

        mContext->BeginScene();
        void * mappedData = mContext->Map( buffer, SubResourceIndex(), MAP_TYPE_READ );
        ASSERT_TRUE( memcmp( mappedData, data.data(), bufferSize ) == 0 );
        mContext->Unmap( buffer, SubResourceIndex() );
        mContext->EndScene();
    }
}

TEST_F(GfwTests, RenderTarget)
{
    // Get default render target

    ConstIRenderTargetRef defaultRenderTarget = mDevice->GetDefaultRenderTarget();

    // Create effect

    IEffectRef fx = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef techDrawRed = fx->GetTechnique( "DrawRed" );
    ConstITechniqueRef techDrawTexturedQuad = fx->GetTechnique( "DrawTexturedQuad" );

    // Create geometry

    float vertPosData[] = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    BufferDesc vertPosBufDesc;
    vertPosBufDesc.type   = BUFFER_VERTEX;
    vertPosBufDesc.size   = sizeof(vertPosData);
    vertPosBufDesc.usage  = USAGE_STATIC;
    IBufferRef vertPosBuf = mDevice->CreateBuffer(vertPosBufDesc, vertPosData);

    // Define vertex attributes

    VertexAttribute vertexAttribs;
    vertexAttribs.semantic = SEMANTIC_POSITION0;
    vertexAttribs.format   = FORMAT_RG32_FLOAT;
    vertexAttribs.stride   = 8;
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout( 1, &vertexAttribs, techDrawRed->GetShader(ShaderStage::VERTEX) );

    // Define draw params

    DrawParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES;
    drawParams.vertexStart = 0;
    drawParams.vertexCount = 3;

    // Create offscreen render target

    const TextureDesc & rtTexDesc = defaultRenderTarget->GetTexture()->GetDesc();
    ITextureRef rtTex = mDevice->CreateTexture(rtTexDesc);

    RenderTargetDesc rtDesc;
    rtDesc.format = rtTexDesc.format;
    IRenderTargetRef renderTarget = mDevice->CreateRenderTarget(rtTex, rtDesc);

    // Main loop

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            // Draw to texture

            ConstIRenderTargetRef rt[] = { renderTarget };
            mContext->SetRenderTargets( 1, rt );
            mContext->Clear( mClearParams );

            mContext->SetInputLayout( inputLayout );
            mContext->SetVertexBuffer( 0, vertPosBuf );

            techDrawRed->Dispatch();
            mContext->Draw(drawParams);

            // Draw screen quad with texture

            mContext->SetRenderTargets(1, &defaultRenderTarget);
            mContext->Clear(mClearParams);

            mContext->SetTexture( ShaderStage::PIXEL, 0, rtTex);

            techDrawTexturedQuad->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        Present();
    }
}
