#include "common/trace.h"

#include "gfw/base/context.h"

#include "gfw/common/device_child.inl"

#include "gfw/core/buffer.h"
#include "gfw/core/functions.h"
#include "gfw/core/resource.h"

namespace GFW {

    using namespace Common;

    static uint32_t GetBufferTarget(BufferType type)
    {
        switch (type)
        {
        case BUFFER_VERTEX:     return GL_ARRAY_BUFFER;
        case BUFFER_INDEX:      return GL_ELEMENT_ARRAY_BUFFER;
        case BUFFER_PIXEL:      return GL_PIXEL_UNPACK_BUFFER;
        default:                TRACE_FAIL();
        }

        return 0;
    }

    Buffer::Buffer(const BufferDesc & desc, IDeviceRef device)
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
            TRACE_ASSERT_GL(glDeleteBuffers, 1, &mHandle);
        }
    }

    bool Buffer::Init( const void * initialData )
    {
        TRACE_ASSERT_GL(glGenBuffers, 1, &mHandle);
        TRACE_ASSERT(mHandle != 0);

        if (mHandle != 0)
        {
            mTarget = GetBufferTarget(mDesc.type);
            uint32_t usage  = GetOGLUsage(mDesc.usage);

            TRACE_ASSERT_GL(glBindBuffer, mTarget, mHandle);
            TRACE_ASSERT_GL(glBufferData, mTarget, mDesc.size, initialData, usage);
            TRACE_ASSERT_GL(glBindBuffer, mTarget, 0);

            return true;
        }

        return false;
    }

    void * Buffer::Map(uint32_t mapFlags)
    {
        TRACE_ASSERT((mapFlags & (MAP_FLAG_READ | MAP_FLAG_WRITE)) != 0);
        TRACE_ASSERT(mDevice->GetCurrentContext().IsAttached());

        uint32_t access;
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

        TRACE_ASSERT_GL(glBindBuffer, mTarget, mHandle);
        void * retVal = TRACE_ASSERT_GL(glMapBuffer, mTarget, access);
        TRACE_ASSERT_GL(glBindBuffer, mTarget, 0);

        return retVal;
    }

    void Buffer::Unmap()
    {
        TRACE_ASSERT(mDevice->GetCurrentContext().IsAttached());

        TRACE_ASSERT_GL(glBindBuffer, mTarget, mHandle);
        TRACE_ASSERT_GL(glUnmapBuffer, mTarget);
        TRACE_ASSERT_GL(glBindBuffer, mTarget, 0);
    }

    void Buffer::UpdateSubresource(const void * data, uint32_t subResourceIndex)
    {
        TRACE_ASSERT(mDevice->GetCurrentContext().IsAttached());

        uint32_t usage = GetOGLUsage(mDesc.usage);

        TRACE_ASSERT_GL(glBindBuffer, mTarget, mHandle);
        TRACE_ASSERT_GL(glBufferData, mTarget, 0, NULL, usage);
        TRACE_ASSERT_GL(glBufferData, mTarget, mDesc.size, data, usage);
        TRACE_ASSERT_GL(glBindBuffer, mTarget, 0);
    }

} // namespace GFW
