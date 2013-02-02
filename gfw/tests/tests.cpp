#include "gfw/gfw.h"
#include "gtest/gtest.h"
#include "common/file.h"

namespace GFWTests {

    using namespace GFW;
    using namespace GFW::Platform;
    using namespace Common;

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

        FileRef vertexShaderSource = File::Create();
        vertexShaderSource->Read("draw.vs");
        IShaderRef vertexShader = device->CreateShader(SHADER_VERTEX, vertexShaderSource->GetData());

        FileRef pixelShaderSource = File::Create();
        pixelShaderSource->Read("draw.ps");
        IShaderRef pixelShader  = device->CreateShader(SHADER_PIXEL, pixelShaderSource->GetData());

        // Create geometry

        float vertices[] = {
            -1.0f, -1.0f, 0.1f, 0.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f
        };

        IBufferRef vertexBuffer = device->CreateBuffer(60, vertices);

        // Define vertex attributes

        VertexAttribute vertexAttribs[] = {
            { "pos",   FORMAT_R32G32_FLOAT,    20, 0, 0 },
            { "color", FORMAT_R32G32B32_FLOAT, 20, 8, 0 }
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

            context->SetShader(vertexShader);
            context->SetShader(pixelShader);

            context->SetVertexAttributes(vertexAttribs);
            context->SetVertexBuffer(0, vertexBuffer);

            context->Draw(drawParams);

            context->Present();
        }
    }

} // namespace GFWTests
