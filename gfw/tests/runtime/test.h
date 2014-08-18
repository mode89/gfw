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
protected:
    static const uint32_t       kBackBufferWidth  = 800;
    static const uint32_t       kBackBufferHeight = 600;
    static const GFW::Format    kBackBufferFormat = GFW::FORMAT_RGBA8_UNORM;

    static unsigned             mFrameCount;

public:
    Test() {}
    Test( const Test & );
    Test & operator= ( const Test & );

    virtual void
    SetUp();

    virtual void
    TearDown();

    virtual void
    InitSwapChain();

    static void
    SetFrameCount( int val ) { mFrameCount = ( val > 0 ) ? val : 1; }

protected:
    GFW::ISwapChainRef          mSwapChain;
    GFW::IDeviceRef             mDevice;
    GFW::ConstIRenderTargetRef  mDefaultRenderTarget;
    GFW::IContextRef            mContext;
    GFW::IFactoryRef            mFactory;
    GFW::ClearParams            mClearParams;
};

class GraphicsTest : public Test
{
public:
    virtual void
    TearDown();

    virtual void
    InitSwapChain();

    void
    Tick();

    void
    Present();

public:
    enum TestMode
    {
        TEST_MODE_NORMAL = 0,
        TEST_MODE_CAPTURE,
        TEST_MODE_COMPARE,
    };

    static void
    SetTestMode( TestMode mode ) { mTestMode = mode; }

    static void
    ShowWindow( bool val ) { mShowWindow = val; }

protected:
    GFW::WindowHandleRef    mWindow;

    static TestMode         mTestMode;
    static bool             mShowWindow;
};

#endif // __GFW_TESTS_GFW_TESTS_H__
