#ifndef __TRACE_H__
#define __TRACE_H__

#include "common\typedefs.h"
#include "common\platform.h"

// Software breakpoint

#if PLATFORM_COMPILER_MSVC
#define TRACE_DEBUG_BREAK()     __debugbreak()
#elif PLATFORM_COMPILER_MINGW
#define TRACE_DEBUG_BREAK()     asm("int $3")
#endif // Determine the compiler

// Failing

#define TRACE_FAIL_MSG(msg) \
    Trace::Message("Failed in %s : line %d : %s\n", __FILE__, __LINE__, msg); \
    TRACE_DEBUG_BREAK() \

#define TRACE_FAIL() TRACE_DEBUG_BREAK()

// Assertion

#if !defined(TRACE_ASSERT_ENABLED) && defined(PLATFORM_DEBUG) // By default enable trace for debug configuration
#define TRACE_ASSERT_ENABLED 1
#endif // !defined(TRACE_ASSERT_ENABLED) && defined(PLATFORM_DEBUG)

#if TRACE_ASSERT_ENABLED

    #define TRACE_ASSERT(expr) \
        if (!(expr)) \
        { \
            Trace::Message("Assertion failed in %s : line %d : Expression %s\n", __FILE__, __LINE__, #expr); \
            TRACE_DEBUG_BREAK(); \
        } \

    #define TRACE_ASSERT_MESSAGE_FORMATED(expr, fmt, ...) \
        if (!(expr)) \
        { \
            Trace::Message("Assertion failed in %s : line %d : " fmt, __FILE__, __LINE__, __VA_ARGS__); \
            TRACE_DEBUG_BREAK(); \
        } \

#else

    #define TRACE_ASSERT(expr)

#endif // TRACE_ASSERT_ENABLED

// Messages

#if !defined(TRACE_MESSAGE_ENABLED) // By default enable trace messages
#define TRACE_MESSAGE_ENABLED 1
#endif // !defined(TRACE_MESSAGE_ENABLED)

#if TRACE_MESSAGE_ENABLED

    #define TRACE_MESSAGE(msg)                  Trace::Message(msg)

    #define TRACE_MESSAGE_FORMATTED(fmt, ...)   Trace::Message(fmt, __VA_ARGS__)

    #if PLATFORM_DEBUG
        #define TRACE_ERROR(msg)                Trace::Message("Error in %s : line %d : %s\n", __FILE__, __LINE__, msg); TRACE_DEBUG_BREAK()
        #define TRACE_ERROR_FORMATTED(fmt, ...) Trace::Message("Error in %s : line %d : " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); TRACE_DEBUG_BREAK()
    #elif PLATFORM_NDEBUG
        #define TRACE_ERROR(msg)                Trace::Message("Error in %s : line %d : %s\n", __FILE__, __LINE__, msg);
        #define TRACE_ERROR_FORMATTED(fmt, ...) Trace::Message("Error in %s : line %d : " fmt "\n", __FILE__, __LINE__, __VA_ARGS__);
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

} // namespace Trace

#endif // __TRACE_H__
