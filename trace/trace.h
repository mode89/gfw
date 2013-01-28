#ifndef __TRACE_H__
#define __TRACE_H__

#include "common\typedefs.h"
#include "common\platform.h"

// By default enable trace for debug configuration

#if !defined(TRACE_ASSERT_ENABLED) && defined(PLATFORM_DEBUG)
#define TRACE_ASSERT_ENABLED 1
#endif // !defined(TRACE_ASSERT_ENABLED) && defined(PLATFORM_DEBUG)

// By default enable trace messages

#if !defined(TRACE_MESSAGE_ENABLED)
#define TRACE_MESSAGE_ENABLED 1
#endif // !defined(TRACE_MESSAGE_ENABLED)

// Software breakpoint

#if PLATFORM_COMPILER_MSVC
#define TRACE_DEBUG_BREAK()     __debugbreak()
#elif PLATFORM_COMPILER_MINGW
#define TRACE_DEBUG_BREAK()     asm("int $3")
#endif // Determine the compiler

// Immediately fail with output a message

#define TRACE_FAIL_MSG(msg) \
    Trace::Message("Failed in %s : line %d : %s\n", __FILE__, __LINE__, msg); \
    TRACE_DEBUG_BREAK() \

#if TRACE_ASSERT_ENABLED

    #define TRACE_ASSERT(expr) \
        if (!expr) \
        { \
            Trace::Message("Assertion failed in %s : line %d : Expression %s\n", __FILE__, __LINE__, #expr); \
            TRACE_DEBUG_BREAK(); \
        } \

    #define TRACE_ASSERT_MESSAGE_FORMATED(expr, fmt, ...) \
        if (!expr) \
        { \
            Trace::Message("Assertion failed in %s : line %d : " fmt, __FILE__, __LINE__, __VA_ARGS__); \
            TRACE_DEBUG_BREAK(); \
        } \

#else

    #define TRACE_ASSERT(expr)

#endif // TRACE_ASSERT_ENABLED

#if TRACE_MESSAGE_ENABLED

    #define TRACE_MESSAGE(msg)          Trace::Message(msg)

    #if PLATFORM_DEBUG
        #define TRACE_MESSAGE_ERROR(msg)    Trace::Message("Error in %s : line %d : %s\n", __FILE__, __LINE__, msg);
    #elif PLATFORM_NDEBUG
        #define TRACE_MESSAGE_ERROR(msg)    Trace::Message("Error in %s : line %d : %s\n", __FILE__, __LINE__, msg); TRACE_DEBUG_BREAK()
    #endif // PLATFORM_DEBUG

#else

    #define TRACE_MESSAGE(msg)
    #define TRACE_MESSAGE_ERROR(msg)

#endif // TRACE_MESSAGE_ENABLED

namespace Trace {

    // Subroutine to show a formatted message

    void Message(const char8_t * format, ...);

} // namespace Trace

#endif // __TRACE_H__
