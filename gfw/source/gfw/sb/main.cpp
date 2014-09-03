#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4127 4242 4265 4308 4310 4365 4371 4619 4625 4626 4640 )
CMN_WARNING_DISABLE_GCC( unused-local-typedefs )
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/vector.hpp"
CMN_WARNING_POP

#include "cmn/trace.h"
#include "gfw/pipeline/scene/common/scene_builder.h"
#include "gfw/shared/scene_binary.h"

using namespace GFW;

void PrintHelp()
{
    std::cout
        << std::endl
        << "  Usage:" << std::endl
        << std::endl
        << "    gfw-sb [options]" << std::endl
        << std::endl
        << "  Options:" << std::endl
        << std::endl
        << "    -s <filename>       Path to the input scene file" << std::endl
        << "    -o <filename>       Path to the compiled scene" << std::endl
        << std::endl;
}

int main( int argc, const char * argv[] )
{
    if ( argc < 2 )
    {
        PrintHelp();
        return -1;
    }

    std::string inputFile;
    std::string outputFile;

    for ( int i = 1; i < argc; ++ i )
    {
        const char * arg = argv[i];
        if ( std::strcmp( "-s", arg ) == 0 )
        {
            if ( ++i < argc )
            {
                inputFile = argv[i];
            }
            else
            {
                CMN_ERR( "Missed filename after -s option" );
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
        SceneBinary sceneBinary;
        ISceneBuilderRef sceneBuilder = CreateSceneBuilderCollada();
        sceneBuilder->Build( sceneBinary, inputFile );

        std::ofstream fileStream( outputFile, std::ios_base::out | std::ios_base::binary );
        {
            boost::archive::text_oarchive archive( fileStream );
            archive << sceneBinary;
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
