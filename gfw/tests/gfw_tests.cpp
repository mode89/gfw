#include "common/counter.h"

#include "gfw_tests.h"

using namespace Common;
using namespace GFW;

void GfwTests::SetUp()
{
    // Create a window

    WindowDesc windowDesc;
    windowDesc.width  = kWindowWidth;
    windowDesc.height = kwindowHeight;

    mWindow = CreateDefaultWindow(windowDesc);

    // Create a graphical mDevice

    DeviceParams deviceParams;
    deviceParams.width        = kWindowWidth;
    deviceParams.height       = kwindowHeight;
    deviceParams.windowHandle = mWindow;

    mDevice = CreateDevice(deviceParams);

    mContext = mDevice->GetDefaultContext();

    // Create a factory

    mFactory = CreateFactory(mDevice);

    // Create clear parameters

    mClearParams.mask     = CLEAR_COLOR;
    mClearParams.color[0] = 0.0f;
    mClearParams.color[1] = 0.0f;
    mClearParams.color[2] = 0.3f;
    mClearParams.color[3] = 1.0f;
}

void GfwTests::TearDown()
{
    DestroyDefaultWindow(mWindow);
}

void GfwTests::Wait()
{
    static uint64_t freq = GetCounterFrequency();

    uint64_t timeStart = GetCounter();
    while ((GetCounter() - timeStart) < (freq / 120));
}
