#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4625 4626 )
#include "gtest/gtest.h"
CMN_WARNING_POP

#include "test.h"
#include <cstring>

int main( int argc, char ** argv )
{
    testing::InitGoogleTest( &argc, argv );

    for ( int i = 0; i < argc; ++ i )
    {
        const char * arg = argv[ i ];
        if ( std::strcmp( arg, "--capture" ) == 0 )
        {
            GraphicsTest::SetTestMode( GraphicsTest::TEST_MODE_CAPTURE );
        }
    }

    return RUN_ALL_TESTS();
}
