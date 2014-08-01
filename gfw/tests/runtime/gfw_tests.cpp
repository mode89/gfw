#include "cmn/counter.h"
#include "gfw_tests.h"
#include "window.h"

using namespace Cmn;
using namespace GFW;

void GfwTests::SetUp()
{
    // Create a window

    WindowDesc windowDesc;
    windowDesc.width  = kWindowWidth;
    windowDesc.height = kwindowHeight;

    mWindow = CreateDefaultWindow(windowDesc);

    // Create GFW factory

    mFactory = CreateFactory();

    // Create a swap chain

    SwapChainDesc swapChainDesc;
    mSwapChain = mFactory->CreateSwapChain( swapChainDesc, mWindow );

    // Create a graphical mDevice

    DeviceParams deviceParams;
    mDevice = mFactory->CreateDevice( deviceParams, mSwapChain );

    mDefaultRenderTarget = mDevice->GetDefaultRenderTarget();

    mContext = mDevice->GetDefaultContext();

    // Create clear parameters

    mClearParams.mask     = CLEAR_COLOR;
    mClearParams.color[0] = 0.0f;
    mClearParams.color[1] = 0.0f;
    mClearParams.color[2] = 0.3f;
    mClearParams.color[3] = 1.0f;
}

void GfwTests::TearDown()
{
    mContext.reset();
    mDevice.reset();
    mFactory.reset();

    DestroyDefaultWindow(mWindow);
}

void GfwTests::Present()
{
    mSwapChain->Present();

    // Wait certain time
    static uint64_t freq = GetCounterFrequency();
    uint64_t timeStart = GetCounter();
    while ((GetCounter() - timeStart) < (freq / 120));
}
