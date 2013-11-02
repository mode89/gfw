#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"

#include <cstring>
#include <iostream>
#include <string>

using namespace GFW;

void PrintHelp()
{
    std::cout
        << "Usage:" << std::endl
        << std::endl
        << "gfw_fxc [options]" << std::endl
        << std::endl
        << "Options:" << std::endl
        << std::endl
        << "-fx <filename>      Path to the FX file" << std::endl
        << "-o <filename>       Path to the compiled effect" << std::endl
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
                TRACE_ERROR( "Missed filename after -fx option" );
                PrintHelp();
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
                TRACE_ERROR( "Missed filename after -o option" );
                PrintHelp();
                return -1;
            }
        }
        else
        {
            TRACE_ERROR_FORMATTED( "Unknown command line argument '%s'", arg );
            PrintHelp();
            return -1;
        }
    }

    EffectBuilderRef effectBuilder = new EffectBuilder;


    return 0;
}
