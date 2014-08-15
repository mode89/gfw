#include "cmn/trace.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/format.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/texture.h"
#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    Texture::Texture( const TextureDesc & desc, const SubResourceData * initialData, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mHandle( 0 )
    {
        VGL( glGenTextures, 1, &mHandle );
        CMN_ASSERT( mHandle != 0 );

        VGL( glBindTexture, GL_TEXTURE_2D, mHandle );
        for ( uint32_t mipLevel = 0; mipLevel < mDesc.mipLevels; ++ mipLevel )
        {
            VGL( glTexImage2D, GL_TEXTURE_2D, 0, GetOGLInternalFormat( desc.format ),
                desc.width, desc.height, 0, GetOGLFormat( desc.format ), GetOGLType( desc.format ),
                initialData ? initialData[ mipLevel ].mem : nullptr );
            VGL( glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        }
        VGL( glBindTexture, GL_TEXTURE_2D, 0 );
    }

    Texture::~Texture()
    {
        if (mHandle != 0)
        {
            VGL( glDeleteTextures, 1, &mHandle );
        }
    }

    void * Texture::Map(uint32_t mapFlags)
    {
        CMN_FAIL_MSG( "Not yet implemented" );
        return NULL;
    }

    void Texture::Unmap()
    {
        CMN_FAIL_MSG( "Not yet implemented" );
    }

    void Texture::UpdateSubresource(const void * data, uint32_t subResourceIndex)
    {
        CMN_FAIL_MSG( "Not yet implemented" );
    }

} // namespace GFW
