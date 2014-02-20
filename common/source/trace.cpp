#include "common\trace.h"

#include <stdio.h>
#include <stdarg.h>

#if PLAT_COMPILER_MSVC
    #include <windows.h>
#endif

namespace Trace {

    void Message(const char8_t * format, ...)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\n");
    }

#if PLAT_COMPILER_MSVC
    void DebugBreak()
    {
        __try {
            __debugbreak();
        }
        __except ( GetExceptionCode() == EXCEPTION_BREAKPOINT ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH ) {}
    }
#endif

} // namespace Trace
