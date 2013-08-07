#include "gfw/gfw.h"

#include "gtest/gtest.h"

#include "common/file.h"
#include "common/counter.h"

using namespace GFW;
using namespace Common;

static const uint32_t WINDOW_WIDTH  = 800;
static const uint32_t WINDOW_HEIGHT = 600;

class GFWTests : public testing::Test
{
public:
    void SetUp()
    {
        // Create a window

        WindowDesc windowDesc;
        windowDesc.width  = WINDOW_WIDTH;
        windowDesc.height = WINDOW_HEIGHT;

        mWindow = CreateDefaultWindow(windowDesc);

        // Create a graphical mDevice

        DeviceParams deviceParams;
        deviceParams.width        = WINDOW_WIDTH;
        deviceParams.height       = WINDOW_HEIGHT;
        deviceParams.windowHandle = mWindow;

        mDevice = CreateDevice(deviceParams);

        mContext = mDevice->GetDefaultContext();

        // Create clear parameters

        mClearParams.mask     = CLEAR_COLOR;
        mClearParams.color[0] = 0.0f;
        mClearParams.color[1] = 0.0f;
        mClearParams.color[2] = 0.3f;
        mClearParams.color[3] = 1.0f;
    }

    void TearDown()
    {
        DestroyDefaultWindow(mWindow);
    }

protected:
    WindowHandle    mWindow;
    IDeviceRef      mDevice;
    IContextRef     mContext;
    ClearParams     mClearParams;
};

void Wait()
{
    static uint64_t freq = GetCounterFrequency();

    uint64_t timeStart = GetCounter();
    while ((GetCounter() - timeStart) < (freq / 120));
}

TEST_F(GFWTests, Clear)
{
    // Main loop

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        mContext->Clear(mClearParams);
        mContext->EndScene();

        mDevice->Present();

        Wait();
    }
}

TEST_F(GFWTests, Draw)
{
    // Create shaders

    FileRef vertexShaderSource = File::Create();
    ASSERT_TRUE(vertexShaderSource->Read(TESTS_SOURCE_DIR "draw_vert.glsl") != 0);

    IShaderRef vertexShader = mDevice->CreateShader(SHADER_VERTEX, vertexShaderSource->GetData());
    ASSERT_TRUE(vertexShader.IsAttached());

    FileRef pixelShaderSource = File::Create();
    ASSERT_TRUE(pixelShaderSource->Read(TESTS_SOURCE_DIR "draw_frag.glsl") != 0);

    IShaderRef pixelShader  = mDevice->CreateShader(SHADER_PIXEL, pixelShaderSource->GetData());
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
    IBufferRef vertexBuffer = mDevice->CreateBuffer(vertexBufferDesc, vertices);

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
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            mContext->Clear(mClearParams);

            mContext->SetShader(SHADER_VERTEX, vertexShader);
            mContext->SetShader(SHADER_PIXEL,  pixelShader);

            mContext->SetVertexAttributes(2, vertexAttribs);
            mContext->SetVertexBuffer(0, vertexBuffer);

            mContext->Draw(drawParams);
        }
        mContext->EndScene();

        mDevice->Present();

        Wait();
    }
}

TEST_F(GFWTests, DISABLED_RenderToTexture)
{
    // Get default color buffer

    IRenderBufferRef defaultColorBuffer = mDevice->GetDefaultColorBuffer();

    // Create shaders

    FileRef vertexShaderSource = File::Create();
    ASSERT_TRUE(vertexShaderSource->Read(TESTS_SOURCE_DIR "render_to_texture_vert.glsl") != 0);

    IShaderRef vertexShader = mDevice->CreateShader(SHADER_VERTEX, vertexShaderSource->GetData());
    ASSERT_TRUE(vertexShader.IsAttached());

    FileRef pixelShaderSource = File::Create();
    ASSERT_TRUE(pixelShaderSource->Read(TESTS_SOURCE_DIR "render_to_texture_frag.glsl") != 0);

    IShaderRef pixelShader  = mDevice->CreateShader(SHADER_PIXEL, pixelShaderSource->GetData());
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
    IBufferRef vertPosBuf = mDevice->CreateBuffer(vertPosBufDesc, vertPosData);

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
    IBufferRef vertColBuf = mDevice->CreateBuffer(vertColBufDesc, vertColData);

    uint16_t indexData[] = {
        0, 1, 3, 2
    };

    BufferDesc indexBufDesc;
    indexBufDesc.type   = BUFFER_INDEX;
    indexBufDesc.size   = sizeof(indexBufDesc);
    indexBufDesc.usage  = USAGE_STATIC_DRAW;
    IBufferRef indexBuf = mDevice->CreateBuffer(indexBufDesc, indexData);

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
    ITextureRef texture = mDevice->CreateTexture(textureDesc);

    IRenderBufferRef colorBuffer = mDevice->CreateRenderBuffer(texture, SubResIdx(0, 0));

    // Main loop

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            // Draw to texture

            mContext->SetFrameBuffer(1, &colorBuffer, NULL);
            mContext->Clear(mClearParams);

            mContext->SetShader(SHADER_VERTEX, vertexShader);
            mContext->SetShader(SHADER_PIXEL,  pixelShader);

            mContext->SetVertexAttributes(2, vertexAttribs);
            mContext->SetVertexBuffer(0, vertPosBuf);
            mContext->SetVertexBuffer(1, vertColBuf);
            mContext->SetIndexBuffer(indexBuf);

            mContext->Draw(drawParams);

            // Draw screen quad with texture

            mContext->SetFrameBuffer(1, &defaultColorBuffer, NULL);
            mContext->Clear(mClearParams);

            mContext->SetTexture(SHADER_PIXEL, 0, texture);

            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        mDevice->Present();

        Wait();
    }
}
