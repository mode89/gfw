#ifndef __COMMON_TRACE_H__
#define __COMMON_TRACE_H__

#include "common\platform.h"
#include <cstdint>
#include <stdexcept>

// Software breakpoint

#if PLAT_DEBUG
    #if PLAT_COMPILER_MSVC
        #define TRACE_DEBUG_BREAK()     __debugbreak()
    #elif PLAT_COMPILER_MINGW
        #define TRACE_DEBUG_BREAK()     asm("int $3")
    #endif // Determine the compiler
#else
    #define TRACE_DEBUG_BREAK()
#endif

// Failing

#if PLAT_DEBUG
    #define TRACE_FAIL_MSG( ... ) \
        Common::TraceError( "%s(%d) : Failed : ", __FILE__, __LINE__ ); \
        Common::TraceError( __VA_ARGS__ ); \
        Common::TraceError( "\n" ); \
        TRACE_DEBUG_BREAK()

    #define TRACE_FAIL() \
        Common::TraceError( "%s(%d) : Failed", __FILE__, __LINE__ ); \
        Common::TraceError( "\n" ); \
        TRACE_DEBUG_BREAK()
#else
    #define TRACE_FAIL_MSG( ... )
    #define TRACE_FAIL()
#endif // PLAT_DEBUG

// Assertion

#if PLAT_DEBUG
    #define TRACE_ASSERT( expr ) \
        if ( !( expr ) ) \
        { \
            Common::TraceError( "%s(%d) : Assertion failed : %s", __FILE__, __LINE__, #expr ); \
            Common::TraceError( "\n" ); \
            TRACE_DEBUG_BREAK(); \
        }

    #define TRACE_ASSERT_MSG( expr, ... ) \
        if ( !( expr ) ) \
        { \
            Common::TraceError( "%s(%d) : Assertion failed : %s : ", __FILE__, __LINE__, #expr ); \
            Common::TraceError( __VA_ARGS__ ); \
            Common::TraceError( "\n" ); \
            TRACE_DEBUG_BREAK(); \
        }

    #define TRACE_ASSERT_AND( expr, var ) \
        var = var && ( expr ); \
        if ( !var ) \
        { \
            Common::TraceError( "%s(%d) : Assertion failed : %s", __FILE__, __LINE__, #expr ); \
            Common::TraceError( "\n" ); \
            TRACE_DEBUG_BREAK(); \
        }
#else
    #define TRACE_ASSERT( expr )
    #define TRACE_ASSERT_MSG( expr, ... )
    #define TRACE_ASSERT_AND( expr, var )
#endif // PLAT_DEBUG

// Messages

#define TRACE_MSG( ... ) \
    Common::TraceMessage( __VA_ARGS__ ); \
    Common::TraceMessage( "\n" )

#define TRACE_ERR( ... ) \
    Common::TraceError( __VA_ARGS__ ); \
    Common::TraceError( "\n" )

// Exceptions

#define TRACE_THROW( ... ) \
    TRACE_DEBUG_BREAK(); \
    throw Common::Exception( __FILE__, __LINE__, __VA_ARGS__ )

namespace Common {

    // Subroutine to show a formatted message

    void TraceMessage( const char * format, ... );

    void TraceError( const char * format, ... );

    class Exception : public std::exception
    {
    public:
        Exception( const char * file, std::uint32_t line, const char * fmt, ... );

        virtual const char *
        what() const PLAT_NOEXCEPT( true ) { return mWhat; }

    private:
        char    mWhat[ 4096 ];
    };

} // namespace Common

#endif // __COMMON_TRACE_H__
