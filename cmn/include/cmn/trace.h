#ifndef __COMMON_TRACE_H__
#define __COMMON_TRACE_H__

#include "cmn\platform.h"
#include <cstdint>
#include <stdexcept>

// Software breakpoint

#if PLAT_DEBUG
    #if PLAT_COMPILER_MSVC
        #define CMN_DEBUG_BREAK()     __debugbreak()
    #elif PLAT_COMPILER_MINGW
        #define CMN_DEBUG_BREAK()     asm("int $3")
    #endif // Determine the compiler
#else
    #define CMN_DEBUG_BREAK()
#endif

// Failing

#if PLAT_DEBUG
    #define CMN_FAIL_MSG( ... ) \
        Cmn::TraceError( "%s(%d) : Failed : ", __FILE__, __LINE__ ); \
        Cmn::TraceError( __VA_ARGS__ ); \
        Cmn::TraceError( "\n" ); \
        CMN_DEBUG_BREAK()

    #define CMN_FAIL() \
        Cmn::TraceError( "%s(%d) : Failed", __FILE__, __LINE__ ); \
        Cmn::TraceError( "\n" ); \
        CMN_DEBUG_BREAK()
#else
    #define CMN_FAIL_MSG( ... )
    #define CMN_FAIL()
#endif // PLAT_DEBUG

// Assertion

#if PLAT_DEBUG
    #define CMN_ASSERT( expr ) \
        if ( !( expr ) ) \
        { \
            Cmn::TraceError( "%s(%d) : Assertion failed : %s", __FILE__, __LINE__, #expr ); \
            Cmn::TraceError( "\n" ); \
            CMN_DEBUG_BREAK(); \
        }

    #define CMN_ASSERT_MSG( expr, ... ) \
        if ( !( expr ) ) \
        { \
            Cmn::TraceError( "%s(%d) : Assertion failed : %s : ", __FILE__, __LINE__, #expr ); \
            Cmn::TraceError( __VA_ARGS__ ); \
            Cmn::TraceError( "\n" ); \
            CMN_DEBUG_BREAK(); \
        }

    #define TRACE_ASSERT_AND( expr, var ) \
        var = var && ( expr ); \
        if ( !var ) \
        { \
            Cmn::TraceError( "%s(%d) : Assertion failed : %s", __FILE__, __LINE__, #expr ); \
            Cmn::TraceError( "\n" ); \
            CMN_DEBUG_BREAK(); \
        }
#else
    #define CMN_ASSERT( expr )
    #define CMN_ASSERT_MSG( expr, ... )
    #define TRACE_ASSERT_AND( expr, var )
#endif // PLAT_DEBUG

// Messages

#define TRACE_MSG( ... ) \
    Cmn::TraceMessage( __VA_ARGS__ ); \
    Cmn::TraceMessage( "\n" )

#define TRACE_ERR( ... ) \
    Cmn::TraceError( __VA_ARGS__ ); \
    Cmn::TraceError( "\n" )

// Exceptions

#define TRACE_THROW( type ) \
    CMN_DEBUG_BREAK(); \
    throw type

#define TRACE_THROW_IF( expr, type ) \
    if ( expr ) \
    { \
        TRACE_THROW( type ); \
    }

namespace Cmn {

    // Subroutine to show a formatted message

    void TraceMessage( const char * format, ... );

    void TraceError( const char * format, ... );

} // namespace Cmn

#endif // __COMMON_TRACE_H__
