#include "cmn/trace.h"

#include "gfw/device_child.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/resource.h"

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
            CMN_FAIL();
        }

        return 0;
    }

} // namespace GFW
