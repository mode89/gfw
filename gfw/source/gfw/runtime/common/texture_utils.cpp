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

    uint32_t GetTextureSize( Format format, uint32_t width, uint32_t height, uint32_t mipCount )
    {
        uint32_t retval = 0;
        uint32_t bpp = GetFormatBitsPerPixel( format );
        while ( ( width > 1 || height > 1 ) && mipCount )
        {
            uint32_t pitch = ( ( width * bpp + 7 ) / 8 + 3 ) & ~0x03;
            uint32_t mipSize = ( height * pitch + 15 ) & ~0x0F;
            retval += mipSize;
            mipCount --;
            if ( width > 1 ) width >>= 1;
            if ( height > 1 ) height >>= 1;
        }
        return retval;
    }

} // namespace GFW
