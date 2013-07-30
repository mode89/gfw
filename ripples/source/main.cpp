#include "common/trace.h"

#include "gfw/gfw.h"

#include <math.h>

using namespace Common;
using namespace GFW;

static const uint32_t kWindowWidth  = 800;
static const uint32_t kWindowHeight = 600;

static const uint32_t kMaxMeshNodeCount = 1000;

int main()
{
    // Initialize the mesh

    uint32_t xNodeDim = static_cast<uint32_t>(sqrtf(kMaxMeshNodeCount * kWindowWidth / kWindowHeight));
    uint32_t yNodeDim = kMaxMeshNodeCount / xNodeDim;
    uint32_t nodeCnt  = xNodeDim * yNodeDim;

    TRACE_MESSAGE_FORMATTED("Node count: %d\n", nodeCnt);

    AutoPointer<float> f = new float [nodeCnt];
    for (uint32_t j = 0; j < yNodeDim; ++ j)
    {
        uint32_t offset = j * xNodeDim;
        for (uint32_t i = 0; i < xNodeDim; ++ i)
        {
            f[i + offset] = 0.0f;
        }
    }

    // Initialize graphics context

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

            // TODO Calculate the mesh
            // TODO Draw the mesh
        }
        context->EndScene();

        device->Present();
    }

    DestroyDefaultWindow(window);

    return 0;
}
