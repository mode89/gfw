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

TEST(GFW, Draw)
{
    IAllocator * allocator = NULL;

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

    // Create shaders

    MemoryBufferRef vertexShaderSource = LoadFile("draw.vs", allocator);
    IShaderRef vertexShader = device->CreateShader(vertexShaderSource->GetData(), SHADER_VERTEX);

    MemoryBufferRef pixelShaderSource = LoadFile("draw.ps", allocator);
    IShaderRef pixelShader  = device->CreateShader(pixelShaderSource->GetData(), SHADER_PIXEL);

    // Create geometry

    float vertices[] = {
        -1.0f, -1.0f, 0.1f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    IBufferRef vertexBuffer = device->CreateBuffer(60, vertices);

    // Define vertex attributes

    VertexAttribute vertexAttribs[] = {
        { "pos",   2, TYPE_FLOAT, 20, 0, 0 },
        { "color", 3, TYPE_FLOAT, 20, 8, 0 }
    };

    // Define draw params

    DrawParams drawParams;
    drawParams.primTop     = PRIM_TRIANGLES;
    drawParams.vertexStart = 0;
    drawParams.vertexCount = 3;

    // Main loop

    uint64_t timeStart = GetPerformanceCounter();
    while ((GetPerformanceCounter() - timeStart) < GetPerformanceCounterFrequency())
    {
        window->Tick();

        context->Clear(cp);

        context->SetShader(SHADER_VERTEX, vertexShader);
        context->SetShader(SHADER_PIXEL, pixelShader);

        context->SetVertexAttributes(vertexAttribs);
        context->SetVertexBuffer(0, vertexBuffer);

        context->Draw(drawParams);

        context->Present();
    }
}
