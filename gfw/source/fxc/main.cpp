#if defined( BOOST_ALL_NO_LIB )
#include "boost/archive/binary_oarchive.hpp"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "boost/serialization/list.hpp"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "boost/serialization/vector.hpp"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "common/trace.h"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "gfw/pipeline/common/effect_builder.h"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "gfw/shared/effect.h"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "gfw/shared/pass.h"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "gfw/shared/shader.h"
#endif
#if defined( BOOST_ALL_NO_LIB )
#include "gfw/shared/technique.h"
#endif

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

    try
    {
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
                    TRACE_ERROR( "Missed filename after -fx option" );
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
                    TRACE_ERROR( "Missed filename after -o option" );
                }
            }
            else
            {
                TRACE_ERROR_FORMATTED( "Unknown command line argument '%s'", arg );
            }
        }
    }
    catch (...)
    {
        return -1;
    }

    TRACE_MESSAGE( "GFW Effect Compiler" );
    TRACE_MESSAGE_FORMATTED( "\tEffect file: %s", fxFile.c_str() );
    TRACE_MESSAGE_FORMATTED( "\tOutput file: %s", outputFile.c_str() );

    EffectBinary effectBinary;

    try
    {
        TRACE_MESSAGE( "\tBuild started" );

        EffectBuilder effectBuilder;
        effectBuilder.Build( effectBinary, fxFile );
    }
    catch (...)
    {
        TRACE_MESSAGE( "\tBuild failed" );

        return -1;
    }

    TRACE_MESSAGE( "\tBuild completed" );

    std::ofstream fileStream( outputFile, std::ios_base::out | std::ios_base::binary );
    {
        TRACE_MESSAGE( "\tSerialization" );

        boost::archive::binary_oarchive archive( fileStream );
        archive << effectBinary;

        TRACE_MESSAGE( "\tSerialization completed" );
    }
    fileStream.close();

    return 0;
}
