#include "gfw/texture_utils.h"

namespace GFW {

    static inline uint32_t TextureUtils_GetMipDim( uint32_t dim, uint32_t mipSlice )
    {
        dim >>= mipSlice;
        return dim ? dim : 1;
    }

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

    uint32_t GetTextureMipWidth( Format format, uint32_t width, uint32_t mipSlice )
    {
        return TextureUtils_GetMipDim( width, mipSlice );
    }

    uint32_t GetTextureMipHeight( Format format, uint32_t height, uint32_t mipSlice )
    {
        return TextureUtils_GetMipDim( height, mipSlice );
    }

    uint32_t GetTextureMipSize( Format format, uint32_t width, uint32_t height, uint32_t mipSlice )
    {
        uint32_t bpp        = GetFormatBitsPerPixel( format );
        uint32_t mipWidth   = TextureUtils_GetMipDim( width, mipSlice );
        uint32_t mipHeight  = TextureUtils_GetMipDim( height, mipSlice );
        uint32_t pitch      = TextureUtils_GetPitch( mipWidth, bpp );
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
