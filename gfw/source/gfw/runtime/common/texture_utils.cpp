#include "gfw/texture_utils.h"

namespace GFW {

    static inline uint32_t TextureUtils_GetPitch( uint32_t width, uint32_t bpp )
    {
        return ( ( width * bpp + 7 ) / 8 + 3 ) & ~0x03; // 4 bytes alignment
    }

    static inline uint32_t TextureUtils_GetSlicePitch( uint32_t height, uint32_t pitch )
    {
        return ( height * pitch + 15 ) & ~0x0F; // 16 bytes alignment
    }

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
        return width ? width : 1;
    }

    uint32_t GetTextureMipHeight( Format format, uint32_t height, uint32_t mipLevel )
    {
        height >>= mipLevel;
        return height ? height : 1;
    }

    uint32_t GetTextureMipSize( Format format, uint32_t width, uint32_t height, uint32_t mipSlice )
    {
        uint32_t bpp = GetFormatBitsPerPixel( format );
        uint32_t mipWidth = GetTextureMipWidth( format, width, mipSlice );
        uint32_t mipHeight = GetTextureMipHeight( format, height, mipSlice );
        uint32_t pitch = TextureUtils_GetPitch( mipWidth, bpp );
        return TextureUtils_GetSlicePitch( mipHeight, pitch );
    }

    uint32_t GetTextureSize( Format format, uint32_t width, uint32_t height, uint32_t mipCount )
    {
        uint32_t retval = 0;
        uint32_t bpp = GetFormatBitsPerPixel( format );
        while ( ( width > 1 || height > 1 ) && mipCount )
        {
            uint32_t pitch = TextureUtils_GetPitch( width, bpp );
            uint32_t mipSize = TextureUtils_GetSlicePitch( height, pitch );
            retval += mipSize;
            mipCount --;
            if ( width > 1 ) width >>= 1;
            if ( height > 1 ) height >>= 1;
        }
        return retval;
    }

} // namespace GFW
