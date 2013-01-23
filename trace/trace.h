#ifndef __TRACE_H__
#define __TRACE_H__

#include "common\typedefs.h"
#include "common\platform.h"

// By default enable trace for debug configuration

#if !defined(TRACE_ASSERT_ENABLED) && defined(PLATFORM_DEBUG)
#define TRACE_ASSERT_ENABLED 1
#endif // !defined(TRACE_ASSERT_ENABLED) && defined(PLATFORM_DEBUG)

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

#else
#define TRACE_ASSERT(expr)
#endif // TRACE_ASSERT_ENABLED

namespace Trace {

    // Subroutine to show a formatted message

    void Message(const char8_t * format, ...);

} // namespace Trace

#endif // __TRACE_H__
