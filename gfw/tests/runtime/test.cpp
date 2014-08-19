#include "cmn/counter.h"
#include "png.h"
#include "test.h"
#include "window.h"
#include <cstring>

using namespace Cmn;
using namespace GFW;

unsigned Test::mFrameCount = 60;

void Test::SetUp()
{
    // Create GFW factory

    mFactory = CreateFactory();

    // Create a swap chain

    InitSwapChain();

    // Create a graphical mDevice

    DeviceParams deviceParams;
    mDevice = mFactory->CreateDevice( deviceParams, mSwapChain );

    mDefaultRenderTarget = mSwapChain->GetBuffer();

    mContext = mDevice->GetDefaultContext();

    // Create clear parameters

    mClearParams.mask     = CLEAR_COLOR;
    mClearParams.color[0] = 0.0f;
    mClearParams.color[1] = 0.0f;
    mClearParams.color[2] = 0.3f;
    mClearParams.color[3] = 1.0f;
}

void Test::TearDown()
{
    mContext.reset();
    mDefaultRenderTarget.reset();
    mDevice.reset();
    mSwapChain.reset();
    mFactory.reset();
}

void Test::InitSwapChain()
{
    SwapChainDesc swapChainDesc;
    swapChainDesc.width = kBackBufferWidth;
    swapChainDesc.height = kBackBufferHeight;
    swapChainDesc.format = kBackBufferFormat;
    mSwapChain = mFactory->CreateSwapChain( swapChainDesc, nullptr );
}

GraphicsTest::TestMode  GraphicsTest::mTestMode     = TEST_MODE_NORMAL;
bool                    GraphicsTest::mShowWindow   = true;

void GraphicsTest::TearDown()
{
    Test::TearDown();
    mWindow.reset();
}

void GraphicsTest::InitSwapChain()
{
    if ( mShowWindow )
    {
        const ::testing::TestInfo * testInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();
        std::string windowTitle =
            std::string( testInfo->test_case_name() ) +
            std::string( "::" ) +
            std::string( testInfo->name() );

        WindowDesc windowDesc;
        windowDesc.width  = kBackBufferWidth;
        windowDesc.height = kBackBufferHeight;

        mWindow = WindowHandleRef( CreateDefaultWindow( windowTitle, windowDesc ),
            [] ( WindowHandle * handle ) {
                if ( handle ) { DestroyDefaultWindow( handle ); }
            } );
    }

    SwapChainDesc swapChainDesc;
    swapChainDesc.width = kBackBufferWidth;
    swapChainDesc.height = kBackBufferHeight;
    swapChainDesc.format = kBackBufferFormat;
    mSwapChain = mFactory->CreateSwapChain( swapChainDesc, mWindow );
}

void GraphicsTest::Tick()
{
    if ( mShowWindow )
    {
        ProcessDefaultWindow( mWindow.get() );
    }
}

void GraphicsTest::Present()
{
    if ( mShowWindow )
    {
        mSwapChain->Present();
    }

    switch ( mTestMode )
    {
    case TEST_MODE_CAPTURE:
        {
            TextureDesc resolveTextureDesc = mDefaultRenderTarget->GetTextureDesc();
            resolveTextureDesc.format = FORMAT_RGBA8_UNORM;
            ITextureRef resolveTexture = mDevice->CreateTexture( resolveTextureDesc );

            TextureDesc stagingTextureDesc = resolveTextureDesc;
            stagingTextureDesc.usage = USAGE_STAGING;
            stagingTextureDesc.cpuAccessFlags = CPU_ACCESS_FLAG_READ;
            ITextureRef stagingTexture = mDevice->CreateTexture( stagingTextureDesc );

            IContextRef context = mDevice->GetDefaultContext();
            context->BeginScene();
            {
                context->SetRenderTargets( 1, &mDefaultRenderTarget );
                context->Resolve( resolveTexture, SubResourceIndex() );
                context->CopyResource( stagingTexture, resolveTexture );
                SubResourceData mappedData;
                context->Map( stagingTexture, SubResourceIndex(), MAP_TYPE_READ, mappedData );
                ASSERT_TRUE( mappedData.mem != nullptr ) << "Failed to map captured image";
                {
                    png_image pngImage;
                    std::memset( &pngImage, 0, sizeof( pngImage ) );
                    pngImage.version = PNG_IMAGE_VERSION;
                    pngImage.width = stagingTextureDesc.width;
                    pngImage.height = stagingTextureDesc.height;
                    pngImage.format = PNG_FORMAT_RGBA;

                    const ::testing::TestInfo * testInfo =
                        ::testing::UnitTest::GetInstance()->current_test_info();
                    std::string pngFileName =
                        std::string( GFW_TESTS_RUNTIME_REFS_DIR ) +
                        std::string( "/" ) +
                        std::string( testInfo->test_case_name() ) +
                        std::string( "_" ) +
                        std::string( testInfo->name() ) +
                        std::string( ".png" );

                    int res = png_image_write_to_file( &pngImage, pngFileName.c_str(), 0,
                        mappedData.mem, static_cast< uint32_t >( mappedData.rowPitch ), nullptr );
                    ASSERT_TRUE( res != 0 ) << "Failed to write captured image";
                }
                context->Unmap( stagingTexture, SubResourceIndex() );
            }
            context->EndScene();
        }
        break;
    case TEST_MODE_COMPARE:
        {
            TextureDesc resolveTextureDesc = mDefaultRenderTarget->GetTextureDesc();
            resolveTextureDesc.format = FORMAT_RGBA8_UNORM;
            ITextureRef resolveTexture = mDevice->CreateTexture( resolveTextureDesc );

            TextureDesc stagingTextureDesc = resolveTextureDesc;
            stagingTextureDesc.usage = USAGE_STAGING;
            stagingTextureDesc.cpuAccessFlags = CPU_ACCESS_FLAG_READ;
            ITextureRef stagingTexture = mDevice->CreateTexture( stagingTextureDesc );

            IContextRef context = mDevice->GetDefaultContext();
            context->BeginScene();
            {
                context->SetRenderTargets( 1, &mDefaultRenderTarget );
                context->Resolve( resolveTexture, SubResourceIndex() );
                context->CopyResource( stagingTexture, resolveTexture );
                SubResourceData mappedData;
                context->Map( stagingTexture, SubResourceIndex(), MAP_TYPE_READ, mappedData );
                ASSERT_TRUE( mappedData.mem != nullptr ) << "Failed to map captured image";
                {
                    png_image pngImage;
                    std::memset( &pngImage, 0, sizeof( pngImage ) );
                    pngImage.version = PNG_IMAGE_VERSION;

                    const ::testing::TestInfo * testInfo =
                        ::testing::UnitTest::GetInstance()->current_test_info();
                    std::string pngFileName =
                        std::string( GFW_TESTS_RUNTIME_REFS_DIR ) +
                        std::string( "/" ) +
                        std::string( testInfo->test_case_name() ) +
                        std::string( "_" ) +
                        std::string( testInfo->name() ) +
                        std::string( ".png" );

                    int res = png_image_begin_read_from_file( &pngImage, pngFileName.c_str() );
                    ASSERT_TRUE( res != 0 ) << "Failed to open reference image";

                    std::shared_ptr< uint8_t > pngBuffer( new uint8_t [ mappedData.slicePitch ],
                        [] ( uint8_t * ptr ) { if ( ptr ) delete[] ptr; } );

                    res = png_image_finish_read( &pngImage, nullptr, pngBuffer.get(),
                        static_cast< uint32_t >( mappedData.rowPitch ), nullptr );
                    ASSERT_TRUE( res != 0 ) << "Failed to read reference image";

                    ASSERT_TRUE( std::memcmp( mappedData.mem, pngBuffer.get(), mappedData.slicePitch ) == 0 )
                        << "Rendered image doesn't correspond with the reference image";
                }
                context->Unmap( stagingTexture, SubResourceIndex() );
            }
            context->EndScene();
        }
        break;
    }

    // Wait certain time
    static uint64_t freq = GetCounterFrequency();
    uint64_t timeStart = GetCounter();
    while ((GetCounter() - timeStart) < (freq / 120));
}
