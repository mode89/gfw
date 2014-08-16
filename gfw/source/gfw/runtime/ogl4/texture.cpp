#include "cmn/trace.h"
#include "gfw/texture_utils.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/format.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/texture.h"
#include "gfw/runtime/common/device_child.inl"
#include <cstdlib>

namespace GFW {

    Texture::Texture( const TextureDesc & desc, const SubResourceData * initialData, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mHandle( 0 )
        , mBufferHandle( 0 )
    {
        if ( mDesc.mipLevels == 0 )
        {
            mDesc.mipLevels = GetTextureMipCount( mDesc.format, mDesc.width, mDesc.height );
        }

        // create texture
        if ( mDesc.usage != USAGE_STAGING )
        {
            VGL( glGenTextures, 1, &mHandle );
            CMN_ASSERT( mHandle != 0 );

            VGL( glBindTexture, GL_TEXTURE_2D, mHandle );
            {
                // allocate graphics memory
                VGL( glTexStorage2D, GL_TEXTURE_2D, mDesc.mipLevels,
                    GetOGLInternalFormat( mDesc.format ), mDesc.width, mDesc.height );

                // update with initial data
                if ( initialData != nullptr )
                {
                    for ( uint32_t mipLevel = 0; mipLevel < mDesc.mipLevels; ++ mipLevel )
                    {
                        uint32_t mipWidth = GetTextureMipWidth( mDesc.format, mDesc.width, mipLevel );
                        uint32_t mipHeight = GetTextureMipHeight( mDesc.format, mDesc.height, mipLevel );
                        VGL( glTexSubImage2D, GL_TEXTURE_2D, mipLevel, 0, 0,
                            mipWidth, mipHeight, GetOGLFormat( mDesc.format ),
                            GetOGLType( desc.format ), initialData[ mipLevel ].mem );
                        VGL( glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                    }
                }
            }
            VGL( glBindTexture, GL_TEXTURE_2D, 0 );
        }

        // create buffer
        if ( mDesc.usage == USAGE_STAGING ||
            ( mDesc.usage == USAGE_DYNAMIC &&
                ( mDesc.cpuAccessFlags & CPU_ACCESS_FLAG_WRITE ) ) )
        {
            VGL( glGenBuffers, 1, &mBufferHandle );
            CMN_ASSERT( mBufferHandle != 0 );

            uint32_t flags = GL_CLIENT_STORAGE_BIT;
            if ( mDesc.cpuAccessFlags & CPU_ACCESS_FLAG_READ )
            {
                flags |= GL_MAP_READ_BIT;
            }
            if ( mDesc.cpuAccessFlags & CPU_ACCESS_FLAG_WRITE )
            {
                flags |= GL_MAP_WRITE_BIT;
            }

            VGL( glBindBuffer, GL_PIXEL_PACK_BUFFER, mBufferHandle );
            {
                uint32_t size = GetTextureSize( mDesc.format, mDesc.width, mDesc.height, mDesc.mipLevels );

                std::shared_ptr< uint8_t > tempData( nullptr,
                    [] ( uint8_t * p ) { if ( p ) delete[] p; } );
                if ( initialData && mDesc.usage == USAGE_STAGING )
                {
                    tempData.reset( new uint8_t [ size ] );
                    size_t offset = 0;
                    for ( uint32_t mipSlice = 0; mipSlice < mDesc.mipLevels; ++ mipSlice )
                    {
                        uint32_t mipSize = GetTextureMipSize( mDesc.format, mDesc.width, mDesc.height, mipSlice );
                        std::memcpy( tempData.get() + offset, initialData[ mipSlice ].mem, mipSize );
                        offset += mipSize;
                    }
                }

                VGL( glBufferStorage, GL_PIXEL_PACK_BUFFER, size, tempData.get(), flags );
            }
            VGL( glBindBuffer, GL_PIXEL_PACK_BUFFER, 0 );
        }
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
