#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4242 4265 4310 4365 4619 4625 4626 4640 )
CMN_WARNING_DISABLE_GCC( unused-local-typedefs )
#include "boost/archive/binary_oarchive.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/vector.hpp"
CMN_WARNING_POP

#include "cmn/trace.h"
#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/shared/effect.h"
#include "gfw/shared/pass.h"
#include "gfw/shared/shader.h"
#include "gfw/shared/technique.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

using namespace GFW;

void PrintHelp()
{
    std::cout
        << std::endl
        << "  Usage:" << std::endl
        << std::endl
        << "    gfw_fxc [options]" << std::endl
        << std::endl
        << "  Options:" << std::endl
        << std::endl
        << "    -fx <filename>      Path to the FX file" << std::endl
        << "    -o <filename>       Path to the compiled effect" << std::endl
        << std::endl;
}

int main( int argc, const char * argv[] )
{
    if ( argc < 2 )
    {
        PrintHelp();
        return -1;
    }

    std::string fxFile;
    std::string outputFile;

    for ( int i = 1; i < argc; ++ i )
    {
        const char * arg = argv[i];
        if ( std::strcmp( "-fx", arg ) == 0 )
        {
            if ( ++i < argc )
            {
                fxFile = argv[i];
            }
            else
            {
                CMN_ERR( "Missed filename after -fx option" );
                return -1;
            }
        }
        else if ( std::strcmp( "-o", arg ) == 0 )
        {
            if ( ++i < argc )
            {
                outputFile = argv[i];
            }
            else
            {
                CMN_ERR( "Missed filename after -o option" );
                return -1;
            }
        }
        else
        {
            CMN_ERR( "Unknown command line argument '%s'", arg );
            return -1;
        }
    }

    try {
        CMN_MSG( "GFW Effect Compiler" );
        CMN_MSG( "\tEffect file: %s", fxFile.c_str() );
        CMN_MSG( "\tOutput file: %s", outputFile.c_str() );

        EffectBinary effectBinary;

        CMN_MSG( "\tBuild started" );

        EffectBuilder effectBuilder;
        effectBuilder.Build( effectBinary, fxFile );

        CMN_MSG( "\tBuild completed" );

        std::ofstream fileStream( outputFile, std::ios_base::out | std::ios_base::binary );
        {
            CMN_MSG( "\tSerialization" );

            boost::archive::binary_oarchive archive( fileStream );
            archive << effectBinary;

            CMN_MSG( "\tSerialization completed" );
        }
        fileStream.close();
    }
    catch ( std::exception & e ) {
        CMN_ERR( e.what() );
        return -1;
    }
    catch ( ... ) {
        CMN_ERR( "Unrecognized exception." );
        return -1;
    }

    return 0;
}