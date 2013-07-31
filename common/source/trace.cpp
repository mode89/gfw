#include "common\trace.h"

#include <stdio.h>
#include <stdarg.h>

namespace Trace {

    void Message(const char8_t * format, ...)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\n");
    }

} // namespace Trace