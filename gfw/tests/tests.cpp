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

    TEST(GFW, DISABLED_Clear)
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

            context->BeginScene();
            context->Clear(cp);
            context->EndScene();

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

            context->BeginScene();
            {
                context->Clear(cp);

                context->SetShader(SHADER_VERTEX, vertexShader);
                context->SetShader(SHADER_PIXEL,  pixelShader);

                context->SetVertexAttributes(2, vertexAttribs);
                context->SetVertexBuffer(0, vertexBuffer);

                context->Draw(drawParams);
            }
            context->EndScene();

            device->Present();

            Wait();
        }

        DestroyDefaultWindow(window);
    }

    TEST(GFW, RenderToTexture)
    {
        // Create a window

        IWindowRef window = CreateEmptyWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        ASSERT_TRUE(window.IsAttached());

        // Create a graphical device

        IDeviceRef device = CreateDevice(window->GetHandle());
        ASSERT_TRUE(device.IsAttached());

        IContextRef context = device->GetDefaultContext();
        ASSERT_TRUE(context.IsAttached());

        // Get default color buffer

        IRenderBufferRef defaultColorBuffer = device->GetDefaultColorBuffer();

        // Create clear parameters

        ClearParams cp;
        cp.mask     = CLEAR_COLOR;
        cp.color[0] = 0.0f;
        cp.color[1] = 0.0f;
        cp.color[2] = 0.3f;
        cp.color[3] = 1.0f;

        // Create shaders

        FileRef vertexShaderSource = File::Create();
        ASSERT_TRUE(vertexShaderSource->Read(TESTS_SOURCE_DIR "render_to_texture_vert.glsl") != 0);

        IShaderRef vertexShader = device->CreateShader(SHADER_VERTEX, vertexShaderSource->GetData());
        ASSERT_TRUE(vertexShader.IsAttached());

        FileRef pixelShaderSource = File::Create();
        ASSERT_TRUE(pixelShaderSource->Read(TESTS_SOURCE_DIR "render_to_texture_frag.glsl") != 0);

        IShaderRef pixelShader  = device->CreateShader(SHADER_PIXEL, pixelShaderSource->GetData());
        ASSERT_TRUE(pixelShader.IsAttached());

        // Create geometry

        float vertPosData[] = {
            -1.0f, -1.0f,
            -1.0f,  1.0f,
             1.0f,  1.0f,
             1.0f, -1.0f
        };

        BufferDesc vertPosBufDesc;
        vertPosBufDesc.type   = BUFFER_VERTEX;
        vertPosBufDesc.size   = sizeof(vertPosData);
        vertPosBufDesc.usage  = USAGE_STATIC_DRAW;
        IBufferRef vertPosBuf = device->CreateBuffer(vertPosBufDesc, vertPosData);

        float vertColData[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f
        };

        BufferDesc vertColBufDesc;
        vertColBufDesc.type   = BUFFER_VERTEX;
        vertColBufDesc.size   = sizeof(vertColData);
        vertColBufDesc.usage  = USAGE_STATIC_DRAW;
        IBufferRef vertColBuf = device->CreateBuffer(vertColBufDesc, vertColData);

        uint16_t indexData[] = {
            0, 1, 3, 2
        };

        BufferDesc indexBufDesc;
        indexBufDesc.type   = BUFFER_INDEX;
        indexBufDesc.size   = sizeof(indexBufDesc);
        indexBufDesc.usage  = USAGE_STATIC_DRAW;
        IBufferRef indexBuf = device->CreateBuffer(indexBufDesc, indexData);

        // Define vertex attributes

        VertexAttribute vertexAttribs[] = {
            { "pos",   FORMAT_R32G32_FLOAT,     8, 0, 0 },
            { "color", FORMAT_R32G32B32_FLOAT, 12, 0, 1 }
        };

        // Define draw params

        DrawIndexedParams drawParams;
        drawParams.primTop    = PRIM_TRIANGLES_STRIP;
        drawParams.indexType  = TYPE_UNSIGNED_SHORT;
        drawParams.indexStart = 0;
        drawParams.indexCount = 4;

        // Create additional color buffer

        TextureDesc textureDesc;
        defaultColorBuffer->GetDesc(textureDesc);
        ITextureRef texture = device->CreateTexture(textureDesc);

        IRenderBufferRef colorBuffer = device->CreateRenderBuffer(texture, SubResIdx(0, 0));

        // Main loop

        for (int i = 0; i < 60; ++ i)
        {
            window->Tick();

            context->BeginScene();
            {
                // Draw to texture

                context->SetFrameBuffer(1, &colorBuffer, NULL);
                context->Clear(cp);

                context->SetShader(SHADER_VERTEX, vertexShader);
                context->SetShader(SHADER_PIXEL,  pixelShader);

                context->SetVertexAttributes(2, vertexAttribs);
                context->SetVertexBuffer(0, vertPosBuf);
                context->SetVertexBuffer(1, vertColBuf);
                context->SetIndexBuffer(indexBuf);

                context->Draw(drawParams);

                // Draw screen quad with texture

                context->SetFrameBuffer(1, &defaultColorBuffer, NULL);
                context->Clear(cp);

                context->SetTexture(SHADER_PIXEL, 0, texture);

                context->DrawScreenQuad();
            }
            context->EndScene();

            device->Present();

            Wait();
        }
    }

} // namespace GFWTests
