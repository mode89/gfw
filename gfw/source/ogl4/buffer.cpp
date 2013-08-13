#include "common/trace.h"

#include "gfw/core/buffer.h"
#include "gfw/core/functions.h"

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

    static uint32_t GetUsage(Usage usage)
    {
        switch (usage)
        {
        case USAGE_STREAM_DRAW:     return GL_STREAM_DRAW;
        case USAGE_STREAM_READ:     return GL_STREAM_READ;
        case USAGE_STREAM_COPY:     return GL_STREAM_COPY;
        case USAGE_STATIC_DRAW:     return GL_STATIC_DRAW;
        case USAGE_STATIC_READ:     return GL_STATIC_READ;
        case USAGE_STATIC_COPY:     return GL_STATIC_COPY;
        case USAGE_DYNAMIC_DRAW:    return GL_DYNAMIC_DRAW;
        case USAGE_DYNAMIC_READ:    return GL_DYNAMIC_READ;
        case USAGE_DYNAMIC_COPY:    return GL_DYNAMIC_COPY;
        default:                    TRACE_FAIL();
        }

        return 0;
    }

    Buffer::Buffer(const BufferDesc & desc)
        : mDesc(desc)
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

            if (initialData != NULL)
            {
                uint32_t usage  = GetUsage(mDesc.usage);

                TRACE_ASSERT_GL(glBindBuffer, mTarget, mHandle);
                TRACE_ASSERT_GL(glBufferData, mTarget, mDesc.size, initialData, usage);
                TRACE_ASSERT_GL(glBindBuffer, mTarget, 0);
            }

            return true;
        }

        return false;
    }

} // namespace GFW
