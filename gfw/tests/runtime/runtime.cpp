#include "test.h"
#include "window.h"

using namespace GFW;

TEST_F( GraphicsTest, Clear )
{
    // Main loop

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

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

TEST_F( GraphicsTest, DrawColored )
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

    SubResourceData initialData;
    initialData.mem = vertices;
    initialData.slicePitch = sizeof( vertices );

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.bindFlags  = BIND_FLAG_VERTEX_BUFFER;
    vertexBufferDesc.size       = sizeof( vertices );
    vertexBufferDesc.usage      = USAGE_STATIC;
    IBufferRef vertexBuffer = mDevice->CreateBuffer( vertexBufferDesc, &initialData );

    // Define vertex attributes

    VertexAttribute vertexAttribs[2];
    vertexAttribs[0].semantic = SEMANTIC_POSITION0;
    vertexAttribs[0].format   = FORMAT_RG32_FLOAT;
    vertexAttribs[0].stride   = 20;
    vertexAttribs[1].semantic = SEMANTIC_COLOR0;
    vertexAttribs[1].format   = FORMAT_RGB32_FLOAT;
    vertexAttribs[1].stride   = 20;
    vertexAttribs[1].offset   = 8;
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout( 2, vertexAttribs, technique->GetShader( SHADER_STAGE_VERTEX ) );

    // Define draw params

    DrawParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES;
    drawParams.vertexStart = 0;
    drawParams.vertexCount = 3;

    // Main loop

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

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

TEST_F( GraphicsTest, DrawIndexed )
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

    SubResourceData vertexData;
    vertexData.mem = vertices;
    vertexData.slicePitch = sizeof( vertices );

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.bindFlags  = BIND_FLAG_VERTEX_BUFFER;
    vertexBufferDesc.size       = sizeof( vertices );
    vertexBufferDesc.usage      = USAGE_STATIC;
    IBufferRef vertexBuffer = mDevice->CreateBuffer( vertexBufferDesc, &vertexData );

    uint16_t indices[] = {
        3, 0, 2, 1
    };

    SubResourceData indexData;
    indexData.mem = indices;
    indexData.slicePitch = sizeof( indices );

    BufferDesc indexBufferDesc;
    indexBufferDesc.bindFlags   = BIND_FLAG_INDEX_BUFFER;
    indexBufferDesc.size        = sizeof( indices );
    indexBufferDesc.usage       = USAGE_STATIC;
    IBufferRef indexBuffer = mDevice->CreateBuffer( indexBufferDesc, &indexData );

    // Define vertex attributes

    VertexAttribute vertexAttribs[2];
    vertexAttribs[0].semantic = SEMANTIC_POSITION0;
    vertexAttribs[0].format   = FORMAT_RG32_FLOAT;
    vertexAttribs[0].stride   = 20;
    vertexAttribs[1].semantic = SEMANTIC_COLOR0;
    vertexAttribs[1].format   = FORMAT_RGB32_FLOAT;
    vertexAttribs[1].stride   = 20;
    vertexAttribs[1].offset   = 8;
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout(2, vertexAttribs, technique->GetShader(SHADER_STAGE_VERTEX));

    // Define draw params

    DrawIndexedParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES_STRIP;
    drawParams.indexType   = TYPE_USHORT;
    drawParams.indexStart  = 0;
    drawParams.indexCount  = 4;

    // Main loop

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

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

TEST_F( GraphicsTest, DrawScreenQuad )
{
    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef tech = effect->GetTechnique( "DrawRedQuad" );

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

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

TEST_F( Test, Reflect )
{
    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
}

struct Vertex
{
    float x;
    float y;
    float z;
};

TEST_F( GraphicsTest, CreateMesh )
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

    SubResourceData vertexData;
    vertexData.mem = vertices.data();
    vertexData.slicePitch = vertices.size() * sizeof( Vertex );

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.size       = sizeof( Vertex ) * vertCnt;
    vertexBufferDesc.bindFlags  = BIND_FLAG_VERTEX_BUFFER;
    vertexBufferDesc.usage      = USAGE_STATIC;
    IBufferRef vertexBuffer = mDevice->CreateBuffer( vertexBufferDesc, &vertexData );

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

    SubResourceData indexData;
    indexData.mem = indices.data();
    indexData.slicePitch = indices.size() * sizeof( uint32_t );

    BufferDesc indexBufferDesc;
    indexBufferDesc.size        = sizeof( uint32_t ) * xSegments * ySegments * 2 * 3;
    indexBufferDesc.bindFlags   = BIND_FLAG_INDEX_BUFFER;
    indexBufferDesc.usage       = USAGE_STATIC;
    IBufferRef indexBuffer = mDevice->CreateBuffer( indexBufferDesc, &indexData );

    // Create input layout

    VertexAttribute vertexAttribute;
    vertexAttribute.semantic = SEMANTIC_POSITION0;
    vertexAttribute.format   = FORMAT_RGB32_FLOAT;
    vertexAttribute.stride   = sizeof(Vertex);
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout(1, &vertexAttribute, effect->GetShader(SHADER_STAGE_VERTEX));

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

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

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

TEST_F( Test, MapBuffer )
{
    static const uint32_t kBufferSize = 1024;

    BufferDesc srcBufferDesc;
    srcBufferDesc.size              = kBufferSize;
    srcBufferDesc.usage             = USAGE_DYNAMIC;
    srcBufferDesc.bindFlags         = BIND_FLAG_VERTEX_BUFFER;
    srcBufferDesc.cpuAccessFlags    = CPU_ACCESS_FLAG_WRITE;
    IBufferRef srcBuffer = mDevice->CreateBuffer( srcBufferDesc );

    BufferDesc dstBufferDesc = srcBufferDesc;
    dstBufferDesc.usage             = USAGE_STAGING;
    dstBufferDesc.cpuAccessFlags    = CPU_ACCESS_FLAG_READ;
    IBufferRef dstBuffer = mDevice->CreateBuffer( dstBufferDesc );

    for ( uint32_t i = 0; i < mFrameCount; ++ i )
    {
        mContext->BeginScene();
        {
            // prepare data
            uint8_t data[ kBufferSize ];
            for ( uint32_t i = 0; i < kBufferSize; ++ i )
            {
                data[ i ] = static_cast< uint8_t >( std::rand() );
            }

            // write source buffer
            SubResourceData srcMappedData;
            mContext->Map( srcBuffer, SubResourceIndex(), MAP_TYPE_WRITE, srcMappedData );
                ASSERT_TRUE( srcMappedData.mem != nullptr );
                memcpy( srcMappedData.mem, data, kBufferSize );
            mContext->Unmap( srcBuffer, SubResourceIndex() );

            // copy source buffer to destination buffer
            mContext->CopyResource( dstBuffer, srcBuffer );

            // read destination buffer
            SubResourceData dstMappedData;
            mContext->Map( dstBuffer, SubResourceIndex(), MAP_TYPE_READ, dstMappedData );
                ASSERT_TRUE( dstMappedData.mem != nullptr );
                ASSERT_TRUE( memcmp( dstMappedData.mem, data, kBufferSize ) == 0 );
            mContext->Unmap( dstBuffer, SubResourceIndex() );
        }
        mContext->EndScene();
    }
}

TEST_F( Test, UpdateBuffer )
{
    static const uint32_t kBufferSize = 1024;

    BufferDesc srcBufferDesc;
    srcBufferDesc.size              = kBufferSize;
    srcBufferDesc.usage             = USAGE_DEFAULT;
    srcBufferDesc.bindFlags         = BIND_FLAG_VERTEX_BUFFER;
    IBufferRef srcBuffer = mDevice->CreateBuffer( srcBufferDesc );

    BufferDesc dstBufferDesc = srcBufferDesc;
    dstBufferDesc.usage             = USAGE_STAGING;
    dstBufferDesc.cpuAccessFlags    = CPU_ACCESS_FLAG_READ;
    IBufferRef dstBuffer = mDevice->CreateBuffer( dstBufferDesc );

    for ( uint32_t i = 0; i < mFrameCount; ++ i )
    {
        mContext->BeginScene();
        {
            // prepare data
            uint8_t data[ kBufferSize ];
            for ( uint32_t i = 0; i < kBufferSize; ++ i )
            {
                data[ i ] = static_cast< uint8_t >( std::rand() );
            }

            // update source buffer
            SubResourceData srcUpdateData;
            srcUpdateData.mem           = data;
            srcUpdateData.slicePitch    = sizeof( data );
            mContext->UpdateSubresource( srcBuffer, SubResourceIndex(), srcUpdateData );

            // copy source buffer to destination buffer
            mContext->CopyResource( dstBuffer, srcBuffer );

            // read destination buffer
            SubResourceData dstMappedData;
            mContext->Map( dstBuffer, SubResourceIndex(), MAP_TYPE_READ, dstMappedData );
                ASSERT_TRUE( dstMappedData.mem != nullptr );
                ASSERT_TRUE( memcmp( dstMappedData.mem, data, kBufferSize ) == 0 );
            mContext->Unmap( dstBuffer, SubResourceIndex() );
        }
        mContext->EndScene();
    }
}

TEST_F( GraphicsTest, RenderTarget )
{
    // Create effect

    IEffectRef fx = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef techDrawRed = fx->GetTechnique( "DrawRedQuad" );
    ConstITechniqueRef techDrawTexturedQuad = fx->GetTechnique( "DrawTexturedQuad" );

    // Create geometry

    float vertices[] = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    SubResourceData vertexData;
    vertexData.mem = vertices;
    vertexData.slicePitch = sizeof( vertices );

    BufferDesc vertPosBufDesc;
    vertPosBufDesc.bindFlags    = BIND_FLAG_VERTEX_BUFFER;
    vertPosBufDesc.size         = sizeof( vertices );
    vertPosBufDesc.usage        = USAGE_STATIC;
    IBufferRef vertPosBuf = mDevice->CreateBuffer( vertPosBufDesc, &vertexData );

    // Define vertex attributes

    VertexAttribute vertexAttribs;
    vertexAttribs.semantic = SEMANTIC_POSITION0;
    vertexAttribs.format   = FORMAT_RG32_FLOAT;
    vertexAttribs.stride   = 8;
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout( 1, &vertexAttribs, techDrawRed->GetShader( SHADER_STAGE_VERTEX ) );

    // Define draw params

    DrawParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES;
    drawParams.vertexStart = 0;
    drawParams.vertexCount = 3;

    // Create offscreen render target

    const TextureDesc & rtTexDesc = mDefaultRenderTarget->GetTextureDesc();
    ITextureRef rtTex = mDevice->CreateTexture( rtTexDesc );

    RenderTargetDesc rtDesc;
    rtDesc.format = rtTexDesc.format;
    IRenderTargetRef renderTarget = mDevice->CreateRenderTarget( rtTex, rtDesc );

    // Main loop

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

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

            mContext->SetRenderTargets( 1, &mDefaultRenderTarget );
            mContext->Clear( mClearParams );

            mContext->SetTexture( SHADER_STAGE_PIXEL, 0, rtTex);

            techDrawTexturedQuad->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F( GraphicsTest, Resolve )
{
    // create effect

    IEffectRef fx = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef techDrawRed = fx->GetTechnique( "DrawRedQuad" );
    ConstITechniqueRef techDrawTexturedQuad = fx->GetTechnique( "DrawTexturedQuad" );

    // create geometry

    float vertices[] = {
        -1.0f,  1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f,
    };

    SubResourceData vertexData;
    vertexData.mem = vertices;
    vertexData.slicePitch = sizeof( vertices );

    BufferDesc vertPosBufDesc;
    vertPosBufDesc.bindFlags    = BIND_FLAG_VERTEX_BUFFER;
    vertPosBufDesc.size         = sizeof( vertices );
    vertPosBufDesc.usage        = USAGE_STATIC;
    IBufferRef vertPosBuf = mDevice->CreateBuffer( vertPosBufDesc, &vertexData );

    // define vertex attributes

    VertexAttribute vertexAttribs;
    vertexAttribs.semantic = SEMANTIC_POSITION0;
    vertexAttribs.format   = FORMAT_RG32_FLOAT;
    vertexAttribs.stride   = 8;
    IInputLayoutRef inputLayout = mDevice->CreateInputLayout( 1,
        &vertexAttribs, techDrawRed->GetShader( SHADER_STAGE_VERTEX ) );

    // define draw params

    DrawParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES;
    drawParams.vertexStart = 0;
    drawParams.vertexCount = 3;

    // create offscreen render target

    const TextureDesc & rtTexDesc = mDefaultRenderTarget->GetTextureDesc();
    ITextureRef rtTex = mDevice->CreateTexture( rtTexDesc );

    RenderTargetDesc rtDesc;
    rtDesc.format = rtTexDesc.format;
    IRenderTargetRef renderTarget = mDevice->CreateRenderTarget( rtTex, rtDesc );

    // create resolve texture

    TextureDesc resolveTextureDesc = mDefaultRenderTarget->GetTextureDesc();
    ITextureRef resolveTexture = mDevice->CreateTexture( resolveTextureDesc );

    // clear parameters

    ClearParams cpBlue;
    cpBlue.color[ 2 ] = 0.3f;
    cpBlue.color[ 3 ] = 1.0f;
    cpBlue.mask = CLEAR_COLOR;

    ClearParams cpPurple;
    cpPurple.color[ 0 ] = 1.0f;
    cpPurple.color[ 2 ] = 1.0f;
    cpPurple.color[ 3 ] = 1.0f;
    cpPurple.mask = CLEAR_COLOR;

    // main loop

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

        mContext->BeginScene();
        {
            // Draw to texture

            mContext->SetRenderTargets( 1, &mDefaultRenderTarget );
            mContext->Clear( cpBlue );

            mContext->SetInputLayout( inputLayout );
            mContext->SetVertexBuffer( 0, vertPosBuf );

            techDrawRed->Dispatch();
            mContext->Draw( drawParams );

            // Resolve

            mContext->Resolve( resolveTexture, SubResourceIndex() );

            // Draw resolved texture

            mContext->SetRenderTargets( 1, &mDefaultRenderTarget );
            mContext->Clear( cpPurple );

            mContext->SetTexture( SHADER_STAGE_PIXEL, 0, resolveTexture );

            techDrawTexturedQuad->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        Present();
    }
}
