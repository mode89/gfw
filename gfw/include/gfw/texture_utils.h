#ifndef __GFW_RUNTIME_COMMON_TEXTURE_UTILS_H__
#define __GFW_RUNTIME_COMMON_TEXTURE_UTILS_H__

#include "gfw/format.h"
#include <cstdint>

namespace GFW {

    uint32_t
    GetTextureMipWidth( Format, uint32_t width, uint32_t mipLevel );

    uint32_t
    GetTextureMipHeight( Format, uint32_t height, uint32_t mipLevel );

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_TEXTURE_UTILS_H__
