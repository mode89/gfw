#include "gfw/texture_utils.h"

namespace GFW {

    uint32_t GetTextureMipCount( Format format, uint32_t width, uint32_t height )
    {
        uint32_t retval = 1;
        width = ( width > height ) ? width : height;
        while ( width > 1 )
        {
            retval ++;
            width >>= 1;
        }
        return retval;
    }

    uint32_t GetTextureMipWidth( Format format, uint32_t width, uint32_t mipLevel )
    {
        width >>= mipLevel;
        if ( width == 0 )
            width = 1;
        return width;
    }

    uint32_t GetTextureMipHeight( Format format, uint32_t height, uint32_t mipLevel )
    {
        height >>= mipLevel;
        if ( height == 0 )
            height = 1;
        return height;
    }

} // namespace GFW
