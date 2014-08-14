#include "cmn/counter.h"
#include "test.h"
#include "window.h"

using namespace Cmn;
using namespace GFW;

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

void GraphicsTest::TearDown()
{
    Test::TearDown();
    mWindow.reset();
}

void GraphicsTest::InitSwapChain()
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

    SwapChainDesc swapChainDesc;
    swapChainDesc.width = kBackBufferWidth;
    swapChainDesc.height = kBackBufferHeight;
    swapChainDesc.format = kBackBufferFormat;
    mSwapChain = mFactory->CreateSwapChain( swapChainDesc, mWindow );
}

void GraphicsTest::Tick()
{
    ProcessDefaultWindow( mWindow.get() );
}

void GraphicsTest::Present()
{
    mSwapChain->Present();

    // Wait certain time
    static uint64_t freq = GetCounterFrequency();
    uint64_t timeStart = GetCounter();
    while ((GetCounter() - timeStart) < (freq / 120));
}
