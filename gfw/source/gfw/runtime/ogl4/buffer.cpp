#include "cmn/trace.h"
#include "gfw/context.h"
#include "gfw/runtime/common/device_child.inl"
#include "gfw/runtime/ogl4/buffer.h"
#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/functions.h"
#include "gfw/runtime/ogl4/resource.h"

namespace GFW {

    static uint32_t GetBufferTarget( unsigned bindFlags )
    {
        if ( bindFlags & BIND_FLAG_VERTEX_BUFFER )
            return GL_ARRAY_BUFFER;
        else if ( bindFlags & BIND_FLAG_INDEX_BUFFER )
            return GL_ELEMENT_ARRAY_BUFFER;
        else if ( bindFlags & BIND_FLAG_CONSTANT_BUFFER )
            return GL_UNIFORM_BUFFER;

        CMN_FAIL();
        return 0;
    }

    Buffer::Buffer(
        const BufferDesc & desc,
        const SubResourceData * initialData,
        DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mHandle( 0 )
        , mTarget( 0 )
    {
        if ( initialData )
        {
            CMN_ASSERT( initialData->slicePitch == mDesc.size );
            CMN_ASSERT( initialData->rowPitch == 0
                || initialData->rowPitch == mDesc.size );
        }

        VGL( glGenBuffers, 1, &mHandle );
        CMN_ASSERT( mHandle != 0 );

        mTarget = GetBufferTarget( mDesc.bindFlags );

        // usage flags
        uint32_t flags = 0;
        if ( mDesc.usage == USAGE_DEFAULT )
        {
            flags |= GL_DYNAMIC_STORAGE_BIT;
        }
        if ( mDesc.cpuAccessFlags & CPU_ACCESS_FLAG_READ )
        {
            flags |= GL_MAP_READ_BIT;
        }
        if ( mDesc.cpuAccessFlags & CPU_ACCESS_FLAG_WRITE )
        {
            flags |= GL_MAP_WRITE_BIT;
        }

        // allocate buffer storage
        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glBufferStorage, mTarget, mDesc.size,
            initialData ? initialData->mem : nullptr, flags );
        VGL( glBindBuffer, mTarget, 0 );
    }

    Buffer::~Buffer()
    {
        if (mHandle != 0)
        {
            VGL( glDeleteBuffers, 1, &mHandle );
        }
    }

    void Buffer::Map( const SubResourceIndex & index, MapType mapType, SubResourceData & mappedData )
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
        mappedData.mem        = VGL( glMapBuffer, mTarget, access );
        mappedData.rowPitch   = mDesc.size;
        mappedData.slicePitch = mDesc.size;
        VGL( glBindBuffer, mTarget, 0 );
    }

    void Buffer::Unmap( const SubResourceIndex & index )
    {
        CMN_ASSERT( index.mipSlice == 0 ); // Buffer doesn't have mip slices
        CMN_ASSERT( index.arraySlice == 0 ); // Buffer doesn't have array slices

        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glUnmapBuffer, mTarget );
        VGL( glBindBuffer, mTarget, 0 );
    }

    void Buffer::UpdateSubresource( const SubResourceIndex & index, const SubResourceData & data )
    {
        CMN_ASSERT( index.mipSlice == 0 ); // Buffer doesn't have mip slices
        CMN_ASSERT( index.arraySlice == 0 ); // Buffer doesn't have array slices
        CMN_ASSERT( data.slicePitch <= mDesc.size );
        CMN_ASSERT( data.rowPitch == 0 || data.rowPitch == data.slicePitch );

        VGL( glBindBuffer, mTarget, mHandle );
        VGL( glBufferSubData, mTarget, 0, data.slicePitch, data.mem );
        VGL( glBindBuffer, mTarget, 0 );
    }

} // namespace GFW
