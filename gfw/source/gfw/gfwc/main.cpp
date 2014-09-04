#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4127 4242 4265 4308 4310 4365 4371 4619 4625 4626 4640 )
CMN_WARNING_DISABLE_GCC( unused-local-typedefs )
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/vector.hpp"
CMN_WARNING_POP

#include "cmn/trace.h"
#include "gfw/pipeline/fx/common/effect_builder.h"
#include "gfw/pipeline/scene/common/scene_builder.h"
#include "gfw/shared/effect.h"
#include "gfw/shared/pass.h"
#include "gfw/shared/scene_binary.h"
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
        << "    gfwc [options]" << std::endl
        << std::endl
        << "  Options:" << std::endl
        << std::endl
        << "    -fx <filename>      Path to the FX file" << std::endl
        << "    -s <filename>       Path to the scene file" << std::endl
        << "    -o <filename>       Path to the output file" << std::endl
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
    std::string sceneFile;
    std::string outputFile;

    for ( int i = 1; i < argc; ++ i )
    {
        const char * arg = argv[i];
        if ( std::strcmp( "-fx", arg ) == 0 )
        {
            if ( ++i < argc )
            {
                if ( !fxFile.empty() || !sceneFile.empty() )
                {
                    CMN_ERR( "Multiple input files are not allowed" );
                    return -1;
                }
                fxFile = argv[i];
            }
            else
            {
                CMN_ERR( "Missed filename after -fx option" );
                return -1;
            }
        }
        else if ( std::strcmp( "-s", arg ) == 0 )
        {
            if ( ++i < argc )
            {
                if ( !fxFile.empty() || !sceneFile.empty() )
                {
                    CMN_ERR( "Multiple input files are not allowed" );
                    return -1;
                }
                sceneFile = argv[i];
            }
            else
            {
                CMN_ERR( "Missed fileanem after -s option" );
                return -1;
            }
        }
        else if ( std::strcmp( "-o", arg ) == 0 )
        {
            if ( ++i < argc )
            {
                if ( !outputFile.empty() )
                {
                    CMN_ERR( "Multiple output files are not allowed" );
                    return -1;
                }
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
        if ( !fxFile.empty() )
        {
            EffectBinary effectBinary;
            EffectBuilder effectBuilder;
            effectBuilder.Build( effectBinary, fxFile );

            std::ofstream fileStream( outputFile, std::ios_base::out | std::ios_base::binary );
            boost::archive::text_oarchive archive( fileStream );
            archive << effectBinary;
        }
        else if ( !sceneFile.empty() )
        {
            SceneBinary sceneBinary;
            ISceneBuilderRef sceneBuilder = CreateSceneBuilderCollada();
            sceneBuilder->Build( sceneBinary, sceneFile );

            std::ofstream fileStream( outputFile, std::ios_base::out );
            boost::archive::text_oarchive archive( fileStream );
            archive << sceneBinary;
        }
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
