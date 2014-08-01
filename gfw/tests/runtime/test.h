#ifndef __GFW_TESTS_GFW_TESTS_H__
#define __GFW_TESTS_GFW_TESTS_H__

#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4625 4626 )
#include "gtest/gtest.h"
CMN_WARNING_POP

#include "gfw/gfw.h"

class Test : public testing::Test
{
private:
    static const uint32_t kWindowWidth  = 800;
    static const uint32_t kwindowHeight = 600;

public:
    Test() {}
    Test( const Test & );
    Test & operator= ( const Test & );

    void SetUp();
    void TearDown();

    void Tick();
    void Present();

protected:
    GFW::WindowHandle           mWindow;
    GFW::ISwapChainRef          mSwapChain;
    GFW::IDeviceRef             mDevice;
    GFW::ConstIRenderTargetRef  mDefaultRenderTarget;
    GFW::IContextRef            mContext;
    GFW::IFactoryRef            mFactory;
    GFW::ClearParams            mClearParams;
};

#endif // __GFW_TESTS_GFW_TESTS_H__
