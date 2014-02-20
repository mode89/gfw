#ifndef __COMMON_TRACE_H__
#define __COMMON_TRACE_H__

#include "common\typedefs.h"
#include "common\platform.h"

// Software breakpoint

#if PLAT_COMPILER_MSVC
#define TRACE_DEBUG_BREAK()     Trace::DebugBreak();
#elif PLAT_COMPILER_MINGW
#define TRACE_DEBUG_BREAK()     asm("int $3")
#endif // Determine the compiler

// Failing

#define TRACE_FAIL_MSG(msg) \
    Trace::Message( "%s(%d) : Failed : %s", __FILE__, __LINE__, msg ); \
    TRACE_DEBUG_BREAK() \

#define TRACE_FAIL() \
    Trace::Message( "%s(%d) : Failed", __FILE__, __LINE__ ); \
    TRACE_DEBUG_BREAK()

// Assertion

#if !defined(TRACE_ASSERT_ENABLED) && defined(PLAT_DEBUG) // By default enable trace for debug configuration
#define TRACE_ASSERT_ENABLED 1
#endif // !defined(TRACE_ASSERT_ENABLED) && defined(PLATFORM_DEBUG)

#if TRACE_ASSERT_ENABLED

    #define TRACE_ASSERT(expr) \
        if (!(expr)) \
        { \
            Trace::Message( "%s(%d) : Assertion failed : %s", __FILE__, __LINE__, #expr ); \
            TRACE_DEBUG_BREAK(); \
        } \

    #define TRACE_ASSERT_MESSAGE_FORMATED(expr, fmt, ...) \
        if (!(expr)) \
        { \
            Trace::Message( "%s(%d) : Assertion failed : %s : " fmt, __FILE__, __LINE__, #expr, __VA_ARGS__ ); \
            TRACE_DEBUG_BREAK(); \
        } \

    #define TRACE_ASSERT_AND(expr, var) \
        var = var && (expr); \
        if (!var) \
        { \
            Trace::Message( "%s(%d) : Assertion failed : %s", __FILE__, __LINE__, #expr ); \
            TRACE_DEBUG_BREAK(); \
        } \

#else

    #define TRACE_ASSERT(expr)
    #define TRACE_ASSERT_MESSAGE_FORMATED(expr, fmt, ...)
    #define TRACE_ASSERT_AND(expr, var)

#endif // TRACE_ASSERT_ENABLED

// Messages

#if !defined(TRACE_MESSAGE_ENABLED) // By default enable trace messages
#define TRACE_MESSAGE_ENABLED 1
#endif // !defined(TRACE_MESSAGE_ENABLED)

#if TRACE_MESSAGE_ENABLED

    #define TRACE_MESSAGE(msg)                  Trace::Message(msg)

    #define TRACE_MESSAGE_FORMATTED(fmt, ...)   Trace::Message(fmt, __VA_ARGS__)

    #if PLAT_DEBUG
        #define TRACE_ERROR(msg)                Trace::Message( "%s(%d) : Error : %s", __FILE__, __LINE__, msg ); TRACE_DEBUG_BREAK()
        #define TRACE_ERROR_FORMATTED(fmt, ...) Trace::Message( "%s(%d) : Error : " fmt, __FILE__, __LINE__, __VA_ARGS__ ); TRACE_DEBUG_BREAK()
    #elif PLAT_NDEBUG
        #define TRACE_ERROR(msg)                Trace::Message( "%s(%d) : Error : %s", __FILE__, __LINE__, msg);
        #define TRACE_ERROR_FORMATTED(fmt, ...) Trace::Message( "%s(%d) : Error : " fmt, __FILE__, __LINE__, __VA_ARGS__ );
    #endif // PLATFORM_DEBUG

#else

    #define TRACE_MESSAGE(msg)
    #define TRACE_MESSAGE_FORMATTED(fmt, ...)
    #define TRACE_ERROR(msg)
    #define TRACE_ERROR_FORMATTED(fmt, ...)

#endif // TRACE_MESSAGE_ENABLED

namespace Trace {

    // Subroutine to show a formatted message

    void Message(const char8_t * format, ...);

#if PLAT_COMPILER_MSVC
    void DebugBreak();
#endif

} // namespace Trace

#endif // __COMMON_TRACE_H__
