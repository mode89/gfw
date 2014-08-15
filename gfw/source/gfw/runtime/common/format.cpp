#include "cmn/trace.h"
#include "gfw/format.h"

namespace GFW {

#define FORMATS             /*  Elem type    Elem num   Bits    */ \
    F(FORMAT_RG32_FLOAT,        TYPE_FLOAT,     2,      64      ) \
    F(FORMAT_RGB32_FLOAT,       TYPE_FLOAT,     3,      96      ) \
    F(FORMAT_RGBA8_UNORM,       TYPE_UBYTE,     4,      32      ) \

#define TYPES               /* Size   */ \
    T(TYPE_FLOAT,               4       ) \
    T(TYPE_USHORT,              2       ) \
    T(TYPE_UINT,                4       ) \
    T(TYPE_UBYTE,               1       ) \

    unsigned GetFormatElementNumber( Format format )
    {
        switch (format)
        {
        #define F( fmt, type, elNum, bits ) case fmt : return elNum;
            FORMATS
        #undef F

        default:
            CMN_FAIL();
        }

        return 0;
    }

    Type GetFormatElementType( Format format )
    {
        switch (format)
        {
        #define F( fmt, type, elNum, bits ) case fmt : return type;
            FORMATS
        #undef F

        default:
            CMN_FAIL();
        }

        return TYPE_UNKNOWN;
    }

    unsigned GetTypeSize( Type type )
    {
        switch (type)
        {
        #define T(t, size) case t : return size;
            TYPES
        #undef T

        default:
            CMN_FAIL();
        }

        return 0;
    }

    unsigned GetFormatBitsPerPixel( Format format )
    {
        switch ( format )
        {
        #define F( fmt, type, elNum, bits ) case fmt : return bits;
            FORMATS
        #undef F
        default:
            CMN_FAIL();
        }
        return 0;
    }

#undef FORMATS
#undef TYPES

} // namespace GFW
