#include "cmn/trace.h"

#include "gfw/context.h"

#include "gfw/runtime/common/device_child.inl"

#include "gfw/runtime/ogl4/buffer.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/resource.h"

namespace GFW {

    static uint32_t GetBufferTarget(BufferType type)
    {
        switch (type)
        {
        case BUFFER_VERTEX:     return GL_ARRAY_BUFFER;
        case BUFFER_INDEX:      return GL_ELEMENT_ARRAY_BUFFER;
        case BUFFER_PIXEL:      return GL_PIXEL_UNPACK_BUFFER;
        default:                CMN_FAIL();
        }

        return 0;
    }

    Buffer::Buffer(const BufferDesc & desc, DeviceIn device)
        : ADeviceChild(device)
        , mDesc(desc)
        , mHandle(0)
        , mTarget(0)
    {

    }

    Buffer::~Buffer()
    {
        if (mHandle != 0)
        {
            VGL( glDeleteBuffers, 1, &mHandle );
        }
    }

    bool Buffer::Init( const void * initialData )
    {
        VGL( glGenBuffers, 1, &mHandle );
        CMN_ASSERT( mHandle != 0 );

        if (mHandle != 0)
        {
            mTarget = GetBufferTarget(mDesc.type);
            uint32_t usage  = GetOGLUsage(mDesc.usage);

            VGL( glBindBuffer, mTarget, mHandle );
            VGL( glBufferData, mTarget, mDesc.size, initialData, usage );
            VGL( glBindBuffer, mTarget, 0 );

            return true;
        }

        return false;
    }

    void * Buffer::Map(uint32_t mapFlags)
    {
        CMN_ASSERT( (mapFlags & (MAP_FLAG_READ | MAP_FLAG_WRITE)) != 0 );
        CMN_ASSERT( mDevice.lock()->GetCurrentContext() );

        uint32_t access = 0;
        if (mapFlags & MAP_FLAG_READ && mapFlags & MAP_FLAG_WRITE)
        {
            access = GL_READ_WRITE;
        }
        else if (mapFlags & MAP_FLAG_READ)
        {
            access = GL_READ_ONLY;
        }
        else if (mapFlags & MAP_FLAG_WRITE)
        {
            access = GL_WRITE_ONLY;
        }

        VGL( glBindBuffer, mTarget, mHandle );
        void * retVal = VGL( glMapBuffer, mTarget, access );
        VGL( glBindBuffer, mTarget, 0 );

        return retVal;
    }

    void Buffer::Unmap()
    {
        CMN_ASSERT( mDevice.lock()->GetCurrentContext() );

        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glUnmapBuffer, mTarget );
        VGL( glBindBuffer, mTarget, 0 );
    }

    void Buffer::UpdateSubresource(const void * data, uint32_t subResourceIndex)
    {
        CMN_ASSERT( mDevice.lock()->GetCurrentContext() );

        uint32_t usage = GetOGLUsage(mDesc.usage);

        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glBufferData, mTarget, 0, NULL, usage );
        VGL( glBufferData, mTarget, mDesc.size, data, usage );
        VGL( glBindBuffer, mTarget, 0 );
    }

} // namespace GFW
