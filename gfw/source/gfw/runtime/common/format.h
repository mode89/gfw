#ifndef __GFW_RUNTIME_COMMON_FORMAT_H__
#define __GFW_RUNTIME_COMMON_FORMAT_H__

#include "gfw/format.h"

namespace GFW {

    uint32_t    GetFormatElementNumber(Format);

    Type        GetFormatElementType(Format);

    uint32_t    GetTypeSize(Type);

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_FORMAT_H__
