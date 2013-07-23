#include "common/trace.h"

#include "gfw/common/format.h"

namespace GFW {

    uint32_t GetFormatElementNumber( Format format )
    {
        switch (format)
        {
        case FORMAT_R32G32B32_FLOAT:    return 3;
        case FORMAT_R32G32_FLOAT:       return 2;
        default:                        return 0;
        }
    }

    Type GetFormatElementType( Format format )
    {
        switch (format)
        {
        case FORMAT_R32G32B32_FLOAT:
        case FORMAT_R32G32_FLOAT:
            return TYPE_FLOAT;
        default:
            return TYPE_UNKNOWN;
        }
    }

} // namespace GFW
