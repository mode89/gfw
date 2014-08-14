#include "test.h"
#include "window.h"

using namespace GFW;

TEST_F( GraphicsTest, Texture )
{
    IEffectRef effect = mFactory->CreateEffect( mDevice, "draw.fxc" );
    ConstITechniqueRef technique = effect->GetTechnique( "DrawTexturedQuad" );

    const uint32_t width  = 32;
    const uint32_t cells  = 4;

    uint32_t pixels[width * width];
    for (uint32_t j = 0; j < width; ++ j)
    {
        for (uint32_t i = 0; i < width; ++ i)
        {
            pixels[i + j * width] = (((i * cells / width) % 2) ^ ((j * cells / width) % 2)) ? 0xFFFFFFFF : 0xFF000000;
        }
    }

    TextureDesc desc;
    desc.format = FORMAT_RGBA8_UNORM;
    desc.width  = width;
    desc.height = width;

    ITextureRef texture = mDevice->CreateTexture(desc, pixels);

    for (int i = 0; i < 60; ++ i)
    {
        Tick();

        mContext->BeginScene();
        {
            mContext->SetRenderTargets(1, &mDefaultRenderTarget);
            mContext->Clear(mClearParams);

            mContext->SetTexture( SHADER_STAGE_PIXEL, 0, texture );
            technique->Dispatch();
            mContext->DrawScreenQuad();
        }
        mContext->EndScene();

        Present();
    }
}
