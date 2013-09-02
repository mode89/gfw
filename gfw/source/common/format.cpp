#include "common/trace.h"

#include "gfw/common/format.h"

namespace GFW {

#define FORMATS             /*  Elem type    Elem num  */ \
    F(FORMAT_R32G32_FLOAT,      TYPE_FLOAT,     2       ) \
    F(FORMAT_R32G32B32_FLOAT,   TYPE_FLOAT,     3       )

#define TYPES               /* Size   */ \
    T(TYPE_FLOAT,               4       ) \
    T(TYPE_UNSIGNED_SHORT,      2       ) \
    T(TYPE_UNSIGNED_INT,        4       ) \

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
