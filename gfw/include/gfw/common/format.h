#ifndef __GFW_COMMON_FORMAT_H__
#define __GFW_COMMON_FORMAT_H__

#include "gfw/base/format.h"

namespace GFW {

    uint32_t    GetFormatElementNumber(Format);

    Type        GetFormatElementType(Format);

    uint32_t    GetTypeSize(Type);

} // namespace GFW

#endif // __GFW_COMMON_FORMAT_H__
