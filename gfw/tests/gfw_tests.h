#ifndef __GFW_TESTS_GFW_TESTS_H__
#define __GFW_TESTS_GFW_TESTS_H__

#include "gtest/gtest.h"
#include "gfw/gfw.h"

class GfwTests : public testing::Test
{
private:
    static const uint32_t kWindowWidth  = 800;
    static const uint32_t kwindowHeight = 600;

public:
    void SetUp();
    void TearDown();

    void Wait();

protected:
    GFW::WindowHandle       mWindow;
    GFW::IDeviceRef         mDevice;
    GFW::IRenderTargetRef   mDefaultRenderTarget;
    GFW::IContextRef        mContext;
    GFW::IFactoryRef        mFactory;
    GFW::ClearParams        mClearParams;
};

#endif // __GFW_TESTS_GFW_TESTS_H__
