#include "cmn/trace.h"

#include "gfw/context.h"

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

    Buffer::Buffer( const BufferDesc & desc, const void * initialData, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mHandle( 0 )
        , mTarget( 0 )
    {
        VGL( glGenBuffers, 1, &mHandle );
        CMN_ASSERT( mHandle != 0 );

        mTarget = GetBufferTarget( mDesc.type );
        uint32_t usage  = GetOGLUsage( mDesc.usage );

        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glBufferData, mTarget, mDesc.size, initialData, usage );
        VGL( glBindBuffer, mTarget, 0 );
    }

    Buffer::~Buffer()
    {
        if (mHandle != 0)
        {
            VGL( glDeleteBuffers, 1, &mHandle );
        }
    }

    void * Buffer::Map( const SubResourceIndex & index, MapType mapType )
    {
        CMN_ASSERT( index.mipSlice == 0 ); // Buffer doesn't have mip slices
        CMN_ASSERT( index.arraySlice == 0 ); // Buffer doesn't have array slices

        uint32_t access = 0;
        switch ( mapType )
        {
        case MAP_TYPE_READ:
            access = GL_READ_ONLY;
            break;
        case MAP_TYPE_WRITE:
            access = GL_WRITE_ONLY;
            break;
        case MAP_TYPE_READ_WRITE:
            access = GL_READ_WRITE;
            break;
        default:
            CMN_FAIL(); // Undefined map-type
        }

        VGL( glBindBuffer, mTarget, mHandle );
        void * retVal = VGL( glMapBuffer, mTarget, access );
        VGL( glBindBuffer, mTarget, 0 );

        return retVal;
    }

    void Buffer::Unmap( const SubResourceIndex & index )
    {
        CMN_ASSERT( index.mipSlice == 0 ); // Buffer doesn't have mip slices
        CMN_ASSERT( index.arraySlice == 0 ); // Buffer doesn't have array slices

        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glUnmapBuffer, mTarget );
        VGL( glBindBuffer, mTarget, 0 );
    }

    void Buffer::UpdateSubresource( const SubResourceIndex & index, const void * data )
    {
        CMN_ASSERT( index.mipSlice == 0 ); // Buffer doesn't have mip slices
        CMN_ASSERT( index.arraySlice == 0 ); // Buffer doesn't have array slices

        uint32_t usage = GetOGLUsage(mDesc.usage);

        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glBufferData, mTarget, 0, NULL, usage );
        VGL( glBufferData, mTarget, mDesc.size, data, usage );
        VGL( glBindBuffer, mTarget, 0 );
    }

} // namespace GFW
