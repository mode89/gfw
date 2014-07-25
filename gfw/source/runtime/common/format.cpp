#include "cmn/trace.h"

#include "gfw/runtime/common/format.h"

namespace GFW {

#define FORMATS             /*  Elem type    Elem num  */ \
    F(FORMAT_RG32_FLOAT,        TYPE_FLOAT,     2       ) \
    F(FORMAT_RGB32_FLOAT,       TYPE_FLOAT,     3       ) \
    F(FORMAT_RGBA8_UNORM,       TYPE_UBYTE,     4       ) \

#define TYPES               /* Size   */ \
    T(TYPE_FLOAT,               4       ) \
    T(TYPE_USHORT,              2       ) \
    T(TYPE_UINT,                4       ) \
    T(TYPE_UBYTE,               1       ) \

    uint32_t GetFormatElementNumber( Format format )
    {
        switch (format)
        {
        #define F(fmt, type, elNum) case fmt : return elNum;
            FORMATS
        #undef F

        default:
            TRACE_FAIL();
        }

        return 0;
    }

    Type GetFormatElementType( Format format )
    {
        switch (format)
        {
        #define F(fmt, type, elNum) case fmt : return type;
            FORMATS
        #undef F

        default:
            TRACE_FAIL();
        }

        return TYPE_UNKNOWN;
    }

    uint32_t GetTypeSize( Type type )
    {
        switch (type)
        {
        #define T(t, size) case t : return size;
            TYPES
        #undef T

        default:
            TRACE_FAIL();
        }

        return 0;
    }

#undef FORMATS
#undef TYPES

} // namespace GFW
