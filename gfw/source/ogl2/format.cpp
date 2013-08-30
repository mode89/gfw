#include "common/trace.h"

#include "gfw/ogl/format.h"
#include "gfw/ogl/functions.h"

namespace GFW {

    uint32_t GetOGLType(Type type)
    {
        switch (type)
        {
        case TYPE_FLOAT:            return GL_FLOAT;
        case TYPE_UNSIGNED_SHORT:   return GL_UNSIGNED_SHORT;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

    uint32_t GetOGLDrawMode(Primitive prim)
    {
        switch (prim)
        {
        case PRIM_TRIANGLES:        return GL_TRIANGLES;
        case PRIM_TRIANGLES_STRIP:  return GL_TRIANGLE_STRIP;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

} // namespace GFW
