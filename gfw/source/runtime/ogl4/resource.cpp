#include "common/trace.h"

#include "gfw/base/device_child.h"
#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/core/resource.h"

namespace GFW {

    uint32_t GetOGLUsage(Usage usage)
    {
        switch (usage)
        {
        case USAGE_DEFAULT:
        case USAGE_STATIC:
            return GL_STATIC_DRAW;
        case USAGE_DYNAMIC:
            return GL_DYNAMIC_DRAW;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

} // namespace GFW
