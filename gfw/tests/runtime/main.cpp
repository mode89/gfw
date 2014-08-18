#include "cmn/platform.h"
#include "cmn/trace.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4625 4626 )
#include "gtest/gtest.h"
CMN_WARNING_POP

#include "test.h"
#include <cstdlib>
#include <cstdio>
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
        else if ( std::strcmp( arg, "--framecount" ) == 0 )
        {
            if ( ++i < argc )
            {
                Test::SetFrameCount( std::atoi( argv[ i ] ) );
            }
            else
            {
                std::printf( "Missed number arfter --framecount option.\n" );
                return -1;
            }
        }
    }

    return RUN_ALL_TESTS();
}
