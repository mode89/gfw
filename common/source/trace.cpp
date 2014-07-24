#include "common\trace.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>

#if PLAT_COMPILER_MSVC
    #include <windows.h>
#endif

namespace Common {

    void TraceMessage( const char * format, ... )
    {
        char message[ 4096 ];

        va_list args;
        va_start( args, format );
        vsprintf( message, format, args );
        va_end( args );

        std::cout << message;
    }

    void TraceError( const char * format, ... )
    {
        char message[ 4096 ];

        va_list args;
        va_start( args, format );
        vsprintf( message, format, args );
        va_end( args );

        std::cerr << message;
    }

} // namespace Trace
