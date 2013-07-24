#include "gfw/gfw.h"

using namespace GFW;

int main()
{
    DeviceParams deviceParams;
    deviceParams.width          = 800;
    deviceParams.height         = 600;
    deviceParams.windowHandle   = NULL; // Force to create a default window

    IDeviceRef device = CreateDevice(deviceParams);

    do 
    {

    } while (device->Present());

    return 0;
}
