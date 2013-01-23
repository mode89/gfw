#include "gfw/gfw.h"
#include "gtest/gtest.h"

using namespace GFW;
using namespace GFW::Platform;

TEST(GFW, ClearDarkBlue)
{
    // Create a graphical device

    IDeviceRef device = CreateDevice();
    ASSERT_TRUE(device.IsAttached());

    // Create a window

	WindowDesc wndDesc;
    wndDesc.width      = 640;
    wndDesc.height     = 480;
    wndDesc.fullScreen = FALSE;

    IWindowRef window = CreateEmptyWindow(wndDesc);
    ASSERT_TRUE(window.IsAttached());

    // Create a graphical context for the window

    IContextRef context = device->CreateContext(window);
    ASSERT_TRUE(context.IsAttached());

    // Create clear parameters

    ClearParams cp;
    cp.mask     = CLEAR_COLOR;
    cp.color[0] = 0.0f;
    cp.color[1] = 0.0f;
    cp.color[2] = 0.3f;
    cp.color[3] = 1.0f;

    // Main loop

    uint64_t timeStart = GetPerformanceCounter();
    while ((GetPerformanceCounter() - timeStart) < GetPerformanceCounterFrequency())
    {
        window->Tick();

        context->Clear(cp);

        context->Present();
    }
}
