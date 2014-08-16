#include "gfw/texture_utils.h"

namespace GFW {

    static inline unsigned TextureUtils_GetMipDim( unsigned dim, unsigned mipSlice )
    {
        dim >>= mipSlice;
        return dim ? dim : 1;
    }

    static inline unsigned TextureUtils_GetPitch( unsigned width, unsigned bpp )
    {
        return ( ( width * bpp + 7 ) / 8 + 3 ) & ~0x03; // 4 bytes alignment
    }

    static inline unsigned TextureUtils_GetSlicePitch( unsigned height, unsigned pitch )
    {
        return ( height * pitch + 15 ) & ~0x0F; // 16 bytes alignment
    }

    unsigned GetTextureMipCount( Format format, unsigned width, unsigned height )
    {
        unsigned retval = 1;
        width = ( width > height ) ? width : height;
        while ( width > 1 )
        {
            retval ++;
            width >>= 1;
        }
        return retval;
    }

    unsigned GetTextureMipWidth( Format format, unsigned width, unsigned mipSlice )
    {
        return TextureUtils_GetMipDim( width, mipSlice );
    }

    unsigned GetTextureMipHeight( Format format, unsigned height, unsigned mipSlice )
    {
        return TextureUtils_GetMipDim( height, mipSlice );
    }

    unsigned GetTextureMipPitch( Format format, unsigned width, unsigned mipSlice )
    {
        unsigned bpp        = GetFormatBitsPerPixel( format );
        unsigned mipWidth   = TextureUtils_GetMipDim( width, mipSlice );
        return TextureUtils_GetPitch( mipWidth, bpp );
    }

    unsigned GetTextureMipSize( Format format, unsigned width, unsigned height, unsigned mipSlice )
    {
        unsigned bpp        = GetFormatBitsPerPixel( format );
        unsigned mipWidth   = TextureUtils_GetMipDim( width, mipSlice );
        unsigned mipHeight  = TextureUtils_GetMipDim( height, mipSlice );
        unsigned pitch      = TextureUtils_GetPitch( mipWidth, bpp );
        return TextureUtils_GetSlicePitch( mipHeight, pitch );
    }

    unsigned GetTextureMipOffset( Format format, unsigned width, unsigned height, unsigned mipSlice )
    {
        unsigned retval = 0;
        unsigned bpp = GetFormatBitsPerPixel( format );
        while ( ( width > 1 || height > 1 ) && mipSlice )
        {
            unsigned pitch = TextureUtils_GetPitch( width, bpp );
            unsigned mipSize = TextureUtils_GetSlicePitch( height, pitch );
            retval += mipSize;
            mipSlice --;
            if ( width > 1 ) width >>= 1;
            if ( height > 1 ) height >>= 1;
        }
        return retval;
    }

    unsigned GetTextureSize( Format format, unsigned width, unsigned height, unsigned mipCount )
    {
        return GetTextureMipOffset( format, width, height, mipCount );
    }

} // namespace GFW
