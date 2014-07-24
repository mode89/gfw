#ifndef __GFW_TESTS_GFW_TESTS_H__
#define __GFW_TESTS_GFW_TESTS_H__

#pragma warning( push )
#pragma warning( disable : 4625 4626 )
#include "gtest/gtest.h"
#pragma warning( pop )

#include "gfw/gfw.h"

class GfwTests : public testing::Test
{
private:
    static const uint32_t kWindowWidth  = 800;
    static const uint32_t kwindowHeight = 600;

public:
    GfwTests() {}
    GfwTests( const GfwTests & );
    GfwTests & operator= ( const GfwTests & );

    void SetUp();
    void TearDown();

    void Wait();

protected:
    GFW::WindowHandle           mWindow;
    GFW::IDeviceRef             mDevice;
    GFW::ConstIRenderTargetRef  mDefaultRenderTarget;
    GFW::IContextRef            mContext;
    GFW::IFactoryRef            mFactory;
    GFW::ClearParams            mClearParams;
};

#endif // __GFW_TESTS_GFW_TESTS_H__
