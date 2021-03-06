#include "test.h"
#include "window.h"
#include <cstring>

using namespace GFW;

TEST_F( GraphicsTest, Texture )
{
    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef technique = effect->GetTechnique( "DrawTexturedQuad" );

    const uint32_t width  = 32;
    const uint32_t cells  = 4;

    uint32_t pixels[width * width];
    for (uint32_t j = 0; j < width; ++ j)
    {
        for (uint32_t i = 0; i < width; ++ i)
        {
            pixels[i + j * width] = (((i * cells / width) % 2) ^ ((j * cells / width) % 2)) ? 0xFFFFFFFF : 0xFF000000;
        }
    }

    SubResourceData initialData;
    initialData.mem         = pixels;
    initialData.slicePitch  = sizeof( pixels );
    initialData.rowPitch    = sizeof( uint32_t ) * width;

    TextureDesc desc;
    desc.format     = FORMAT_RGBA8_UNORM;
    desc.width      = width;
    desc.height     = width;
    desc.mipLevels  = 1;

    ITextureRef texture = mDevice->CreateTexture( desc, &initialData );

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

        mContext->BeginScene();
        {
            mContext->SetRenderTargets(1, &mDefaultRenderTarget);
            mContext->Clear(mClearParams);

            mContext->SetTexture( SHADER_STAGE_PIXEL, 0, texture );
            technique->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F( GraphicsTest, MapWriteDynamicTexture )
{
    static const uint32_t   kTextureWidth   = 64;
    static const Format     kTextureFormat  = FORMAT_RGBA8_UNORM;

    TextureDesc textureDesc;
    textureDesc.usage           = USAGE_DYNAMIC;
    textureDesc.cpuAccessFlags  = CPU_ACCESS_FLAG_WRITE;
    textureDesc.format          = kTextureFormat;
    textureDesc.width           = kTextureWidth;
    textureDesc.height          = kTextureWidth;
    textureDesc.mipLevels       = 1;
    ITextureRef texture = mDevice->CreateTexture( textureDesc );

    mContext->BeginScene();
    {
        SubResourceData mappedData;
        mContext->Map( texture, SubResourceIndex(), MAP_TYPE_WRITE, mappedData );
        {
            ASSERT_TRUE( mappedData.mem != nullptr );
            ASSERT_TRUE( mappedData.rowPitch == kTextureWidth * sizeof( uint32_t ) );
            ASSERT_TRUE( mappedData.slicePitch == kTextureWidth * mappedData.rowPitch );

            uint32_t * ptr = static_cast< uint32_t * >( mappedData.mem );
            for ( int i = 0, n = kTextureWidth * kTextureWidth; i < n; ++ i )
            {
                ptr[ i ] = 0xFF00FF00;
            }
        }
        mContext->Unmap( texture, SubResourceIndex() );
    }
    mContext->EndScene();

    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef technique = effect->GetTechnique( "DrawTexturedQuad" );

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        Tick();

        mContext->BeginScene();
        {
            mContext->SetRenderTargets( 1, &mDefaultRenderTarget );
            mContext->Clear( mClearParams );

            mContext->SetTexture( SHADER_STAGE_PIXEL, 0, texture );

            technique->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        Present();
    }
}

TEST_F( Test, MapTexture )
{
    static const uint32_t   kTextureWidth   = 64;
    static const Format     kTextureFormat  = FORMAT_RGBA8_UNORM;

    TextureDesc srcTextureDesc;
    srcTextureDesc.format           = kTextureFormat;
    srcTextureDesc.width            = kTextureWidth;
    srcTextureDesc.height           = kTextureWidth;
    srcTextureDesc.mipLevels        = 1;
    srcTextureDesc.usage            = USAGE_DYNAMIC;
    srcTextureDesc.cpuAccessFlags   = CPU_ACCESS_FLAG_WRITE;
    ITextureRef srcTexture = mDevice->CreateTexture( srcTextureDesc );

    TextureDesc dstTextureDesc = srcTextureDesc;
    dstTextureDesc.usage            = USAGE_STAGING;
    dstTextureDesc.cpuAccessFlags   = CPU_ACCESS_FLAG_READ;
    ITextureRef dstTexture = mDevice->CreateTexture( dstTextureDesc );

    for ( unsigned i = 0; i < mFrameCount; ++ i )
    {
        mContext->BeginScene();
        {
            uint32_t sampleData[ kTextureWidth * kTextureWidth ];
            for ( int i = 0, n = kTextureWidth * kTextureWidth; i < n; ++ i )
            {
                sampleData[ i ] = static_cast< uint32_t >( std::rand() );
            }

            // write source texture
            SubResourceData srcMappedData;
            mContext->Map( srcTexture, SubResourceIndex(), MAP_TYPE_WRITE, srcMappedData );
                ASSERT_TRUE( srcMappedData.mem != nullptr );
                std::memcpy( srcMappedData.mem, sampleData, sizeof( sampleData ) );
            mContext->Unmap( srcTexture, SubResourceIndex() );

            // copy source texture to destination texture
            mContext->CopyResource( dstTexture, srcTexture );

            // read destination texture
            SubResourceData dstMappedData;
            mContext->Map( dstTexture, SubResourceIndex(), MAP_TYPE_READ, dstMappedData );
                ASSERT_TRUE( dstMappedData.mem != nullptr );
                ASSERT_TRUE( std::memcmp( dstMappedData.mem, sampleData, dstMappedData.slicePitch ) == 0 );
            mContext->Unmap( dstTexture, SubResourceIndex() );
        }
        mContext->EndScene();
    }
}
