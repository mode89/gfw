#include "gfw/gfw.h"

#include "gtest/gtest.h"

#include "common/file.h"
#include "common/counter.h"

namespace GFWTests {

    using namespace GFW;
    using namespace Common;

    static const uint32_t WINDOW_WIDTH  = 800;
    static const uint32_t WINDOW_HEIGHT = 600;

    void Wait()
    {
        static uint64_t freq = GetCounterFrequency();

        uint64_t timeStart = GetCounter();
        while ((GetCounter() - timeStart) < (freq / 120));
    }

    TEST(GFW, ClearDarkBlue)
    {
        // Create a window

        WindowDesc windowDesc;
        windowDesc.width  = WINDOW_WIDTH;
        windowDesc.height = WINDOW_HEIGHT;

        WindowHandle window = CreateDefaultWindow(windowDesc);
        ASSERT_TRUE(window != NULL);

        // Create a graphical device

        DeviceParams deviceParams;
        deviceParams.width        = WINDOW_WIDTH;
        deviceParams.height       = WINDOW_HEIGHT;
        deviceParams.windowHandle = window;

        IDeviceRef device = CreateDevice(deviceParams);
        ASSERT_TRUE(device.IsAttached());

        // Create a graphical context

        IContextRef context = device->CreateContext();
        ASSERT_TRUE(context.IsAttached());

        // Create clear parameters

        ClearParams cp;
        cp.mask     = CLEAR_COLOR;
        cp.color[0] = 0.0f;
        cp.color[1] = 0.0f;
        cp.color[2] = 0.3f;
        cp.color[3] = 1.0f;

        // Main loop

        for (int i = 0; i < 60; ++ i)
        {
            ProcessDefaultWindow(window);

            context->Clear(cp);

            device->Present();

            Wait();
        }

        DestroyDefaultWindow(window);
    }

    TEST(GFW, Draw)
    {
        // Create a window

        WindowDesc windowDesc;
        windowDesc.width  = WINDOW_WIDTH;
        windowDesc.height = WINDOW_HEIGHT;

        WindowHandle window = CreateDefaultWindow(windowDesc);
        ASSERT_TRUE(window != NULL);

        // Create a graphical device

        DeviceParams deviceParams;
        deviceParams.width        = WINDOW_WIDTH;
        deviceParams.height       = WINDOW_HEIGHT;
        deviceParams.windowHandle = window;

        IDeviceRef device = CreateDevice(deviceParams);
        ASSERT_TRUE(device.IsAttached());

        // Create a graphical context

        IContextRef context = device->CreateContext();
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
        ASSERT_TRUE(vertexShaderSource->Read(TESTS_SOURCE_DIR "draw_vert.glsl") != 0);

        IShaderRef vertexShader = device->CreateShader(SHADER_VERTEX, vertexShaderSource->GetData());
        ASSERT_TRUE(vertexShader.IsAttached());

        FileRef pixelShaderSource = File::Create();
        ASSERT_TRUE(pixelShaderSource->Read(TESTS_SOURCE_DIR "draw_frag.glsl") != 0);

        IShaderRef pixelShader  = device->CreateShader(SHADER_PIXEL, pixelShaderSource->GetData());
        ASSERT_TRUE(pixelShader.IsAttached());

        // Create geometry

        float vertices[] = {
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f
        };

        BufferDesc vertexBufferDesc;
        vertexBufferDesc.type = BUFFER_VERTEX;
        vertexBufferDesc.size = 60;
        vertexBufferDesc.usage = USAGE_STATIC_DRAW;
        IBufferRef vertexBuffer = device->CreateBuffer(vertexBufferDesc, vertices);

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

        for (int i = 0; i < 60; ++ i)
        {
            ProcessDefaultWindow(window);

            context->Clear(cp);

            context->SetShader(SHADER_VERTEX, vertexShader);
            context->SetShader(SHADER_PIXEL,  pixelShader);

            context->SetVertexAttributes(2, vertexAttribs);
            context->SetVertexBuffer(0, vertexBuffer);

            context->Draw(drawParams);

            device->Present();

            Wait();
        }

        DestroyDefaultWindow(window);
    }

} // namespace GFWTests
