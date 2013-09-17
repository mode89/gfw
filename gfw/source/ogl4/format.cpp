#include "common/trace.h"

#include "gfw/common/format.h"

#include "gfw/core/format.h"
#include "gfw/core/functions.h"

namespace GFW {

#define FORMATS \
    F(FORMAT_RGB32_FLOAT,   GL_RGB32F,      GL_RGB)     \
    F(FORMAT_RG32_FLOAT,    GL_RG32F,       GL_RG)      \
    F(FORMAT_RGBA8_UNORM,   GL_RGBA8,       GL_RGBA)    \

    uint32_t GetOGLType(Type type)
    {
        switch (type)
        {
        case TYPE_FLOAT:
            return GL_FLOAT;
        case TYPE_USHORT:
            return GL_UNSIGNED_SHORT;
        case TYPE_UINT:
            return GL_UNSIGNED_INT;
        case TYPE_UBYTE:
            return GL_UNSIGNED_BYTE;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

    uint32_t GetOGLType(Format format)
    {
        return GetOGLType(GetFormatElementType(format));
    }

    uint32_t GetOGLDrawMode(Primitive prim)
    {
        switch (prim)
        {
        case PRIM_TRIANGLES:
            return GL_TRIANGLES;
        case PRIM_TRIANGLES_STRIP:
            return GL_TRIANGLE_STRIP;
        default:
            TRACE_FAIL();
        }

        return 0;
    }

    uint32_t GetOGLInternalFormat(Format format)
    {
        switch (format)
        {
#define F(gfwFmt, intFmt, fmt) case gfwFmt : return intFmt;
            FORMATS
#undef F

        default:
            TRACE_FAIL_MSG("Unsupported format");
        }

        return 0;
    }

    uint32_t GetOGLFormat(Format format)
    {
        switch (format)
        {
#define F(gfwFmt, intFmt, fmt) case gfwFmt : return fmt;
            FORMATS
#undef F

        default:
            TRACE_FAIL_MSG("Unsupported format");
        }

        return 0;
    }

} // namespace GFW
