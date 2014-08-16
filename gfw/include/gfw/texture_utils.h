#ifndef __GFW_RUNTIME_COMMON_TEXTURE_UTILS_H__
#define __GFW_RUNTIME_COMMON_TEXTURE_UTILS_H__

#include "gfw/format.h"

namespace GFW {

    unsigned
    GetTextureMipCount( Format, unsigned width, unsigned height );

    unsigned
    GetTextureMipWidth( Format, unsigned width, unsigned mipSlice );

    unsigned
    GetTextureMipHeight( Format, unsigned height, unsigned mipSlice );

    unsigned
    GetTextureMipPitch( Format, unsigned width, unsigned mipSlice );

    unsigned
    GetTextureMipSize( Format, unsigned height, unsigned width, unsigned mipSlice );

    unsigned
    GetTextureMipOffset( Format, unsigned height, unsigned width, unsigned mipSlice );

    unsigned
    GetTextureSize( Format, unsigned width, unsigned height, unsigned mipCount );

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_TEXTURE_UTILS_H__
