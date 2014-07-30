#include "cmn/counter.h"

#include "gfw_tests.h"

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

    // Create a graphical mDevice

    DeviceParams deviceParams;
    deviceParams.backBufferWidth    = kWindowWidth;
    deviceParams.backBufferHeight   = kwindowHeight;
    deviceParams.backBufferFormat   = FORMAT_RGBA8_UNORM;
    deviceParams.windowHandle       = mWindow;

    mDevice = mFactory->CreateDevice( deviceParams );

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

void GfwTests::Wait()
{
    static uint64_t freq = GetCounterFrequency();

    uint64_t timeStart = GetCounter();
    while ((GetCounter() - timeStart) < (freq / 120));
}
