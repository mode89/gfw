#include "gfw_tests.h"

using namespace GFW;

TEST_F(GfwTests, Texture)
{
    IEffectRef effect = mFactory->CreateEffect(TESTS_SOURCE_DIR "draw_texture.fx");

    uint32_t pixels[] = {
        0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF, 0xFF000000,
        0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF, 0xFF000000
    };

    TextureDesc desc;
    desc.format = FORMAT_RGBA8_UNORM;
    desc.width  = 4;
    desc.height = 4;

    ITextureRef texture = mDevice->CreateTexture(desc, pixels);

    for (int i = 0; i < 60; ++ i)
    {
        ProcessDefaultWindow(mWindow);

        mContext->BeginScene();
        {
            mContext->Clear(mClearParams);
            mContext->SetTexture(SHADER_STAGE_PIXEL, 0, texture);
            effect->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        mDevice->Present();

        Wait();
    }
}
