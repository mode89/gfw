#include "gfw/gfw.h"
#include "gtest/gtest.h"

#include "common/file.h"

namespace GFWTests {

    using namespace GFW;
    using namespace GFW::Platform;
    using namespace Common;

    void Wait()
    {
        static uint64_t freq = GetPerformanceCounterFrequency();

        uint64_t timeStart = GetPerformanceCounter();
        while ((GetPerformanceCounter() - timeStart) < (freq / 120));
    }

    TEST(GFW, Clear)
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

        for (int i = 0; i < 60; ++ i)
        {
            window->Tick();

            context->Clear(cp);

            context->Present();

            Wait();
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
            window->Tick();

            context->Clear(cp);

            context->SetShader(SHADER_VERTEX, vertexShader);
            context->SetShader(SHADER_PIXEL,  pixelShader);

            context->SetVertexAttributes(2, vertexAttribs);
            context->SetVertexBuffer(0, vertexBuffer);

            context->Draw(drawParams);

            context->Present();

            Wait();
        }
    }

    TEST(GFW, RenderToTexture)
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

        // Get default color buffer

        IRenderBufferRef defaultColorBuffer = context->GetDefaultColorBuffer();

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
        vertColBufDesc.type = BUFFER_VERTEX;
        vertColBufDesc.size = sizeof(vertColData);
        vertColBufDesc.usage = USAGE_STATIC_DRAW;
        IBufferRef vertColBuf = device->CreateBuffer(vertColBufDesc, vertColData);

        uint16_t indexData[] = {
            0, 1, 3, 2
        };

        BufferDesc indexBufDesc;
        indexBufDesc.type = BUFFER_INDEX;
        indexBufDesc.size = sizeof(indexBufDesc);
        indexBufDesc.usage = USAGE_STATIC_DRAW;
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

        IRenderBufferRef colorBuffer = device->CreateColorBuffer(texture, SubResIdx(0, 0));

        // Main loop

        for (int i = 0; i < 60; ++ i)
        {
            window->Tick();

            // Draw to texture

            context->BuildFramebuffer(1, &colorBuffer, NULL);
            context->Clear(cp);

            context->SetShader(SHADER_VERTEX, vertexShader);
            context->SetShader(SHADER_PIXEL,  pixelShader);

            context->SetVertexAttributes(2, vertexAttribs);
            context->SetVertexBuffer(0, vertPosBuf);
            context->SetVertexBuffer(1, vertColBuf);
            context->SetIndexBuffer(indexBuf);

            context->Draw(drawParams);

            // Draw screen quad with texture

            context->BuildFramebuffer(1, &defaultColorBuffer, NULL);
            context->Clear(cp);

            context->SetTexture(SHADER_PIXEL, 0, texture);

            context->DrawScreenQuad();

            context->Present();

            Wait();
        }
    }

} // namespace GFWTests
