#include "common/trace.h"
#include "gfw/runtime/core/device.h"
#include "gfw/runtime/core/format.h"
#include "gfw/runtime/core/functions.h"
#include "gfw/runtime/core/texture.h"
#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    Texture::Texture( const TextureDesc & desc, const void * initialData, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mHandle( 0 )
    {
        TRACE_ASSERT_GL(glGenTextures, 1, &mHandle);
        TRACE_ASSERT(mHandle != 0);

        TRACE_ASSERT_GL(glBindTexture, GL_TEXTURE_2D, mHandle);
        TRACE_ASSERT_GL(glTexImage2D,
            GL_TEXTURE_2D, 0, GetOGLInternalFormat(desc.format), desc.width, desc.height, 0,
            GetOGLFormat(desc.format), GetOGLType(desc.format), initialData);
        TRACE_ASSERT_GL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        TRACE_ASSERT_GL(glBindTexture, GL_TEXTURE_2D, 0);
    }

    Texture::~Texture()
    {
        if (mHandle != 0)
        {
            TRACE_ASSERT_GL(glDeleteTextures, 1, &mHandle);
        }
    }

    void * Texture::Map(uint32_t mapFlags)
    {
        TRACE_FAIL_MSG("Not yet implemented");
        return NULL;
    }

    void Texture::Unmap()
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

    void Texture::UpdateSubresource(const void * data, uint32_t subResourceIndex)
    {
        TRACE_FAIL_MSG("Not yet implemented");
    }

} // namespace GFW
