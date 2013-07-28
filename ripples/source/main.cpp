#include "common/trace.h"

#include "gfw/gfw.h"

using namespace GFW;

static const uint32_t kWindowWidth  = 800;
static const uint32_t kWindowHeight = 600;

int main()
{
    WindowDesc windowDesc;
    windowDesc.width  = kWindowWidth;
    windowDesc.height = kWindowHeight;
    WindowHandle window = CreateDefaultWindow(windowDesc);

    DeviceParams deviceParams;
    deviceParams.width          = kWindowWidth;
    deviceParams.height         = kWindowHeight;
    deviceParams.windowHandle   = window;

    IDeviceRef device = CreateDevice(deviceParams);

    IContextRef context = device->GetDefaultContext();

    while (ProcessDefaultWindow(window))
    {
        context->BeginScene();
        {
            static ClearParams clearParams;
            clearParams.mask  = CLEAR_COLOR;
            clearParams.color[0] = 0.0f;
            clearParams.color[1] = 0.0f;
            clearParams.color[2] = 0.3f;
            clearParams.color[3] = 1.0f;

            context->Clear(clearParams);
        }
        context->EndScene();

        device->Present();
    }

    DestroyDefaultWindow(window);

    return 0;
}
