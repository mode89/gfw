#ifndef __GFW_BASE_RESOURCE_H__
#define __GFW_BASE_RESOURCE_H__

#include "gfw/types_fwd.h"
#include "gfw/device_child.h"

namespace GFW {

    enum Usage
    {
        USAGE_DEFAULT = 0,      // Only read/write access by GPU is allowed (draw, copy to/from)
        USAGE_STATIC,           // Only read access by GPU is allowed (draw, copy from)
        USAGE_DYNAMIC,          // Can only be read by GPU (draw, copy from) and written by CPU (update, map write)
        USAGE_STAGING           // Can be read/written by GPU (copy to/from) and CPU (update, map read/write)
    };

    enum CpuAccessFlag
    {
        CPU_ACCESS_FLAG_READ    = ( 1 << 0 ),   // only staging resources
        CPU_ACCESS_FLAG_WRITE   = ( 1 << 1 ),   // only staging or dynamic resources
    };

    struct SubResourceIndex
    {
        uint32_t    mipSlice;
        uint32_t    arraySlice;

        SubResourceIndex()
            : mipSlice( 0 )
            , arraySlice( 0 )
        {}

        SubResourceIndex( uint32_t m, uint32_t s )
            : mipSlice( m )
            , arraySlice( s )
        {}
    };

    struct SubResourceData
    {
        void *      mem;
        uint32_t    rowPitch;
        uint32_t    slicePitch;

        SubResourceData()
            : mem( nullptr )
            , rowPitch( 0 )
            , slicePitch( 0 )
        {}
    };

    enum MapType : uint32_t
    {
        MAP_TYPE_UNKNOWN = 0,
        MAP_TYPE_READ,
        MAP_TYPE_WRITE,
        MAP_TYPE_READ_WRITE,
    };

    enum ResourceType : uint32_t
    {
        RESOURCE_TYPE_UNKNOWN = 0,
        RESOURCE_TYPE_BUFFER,
        RESOURCE_TYPE_TEXTURE,
    };

    struct ResourceDesc
    {
        Usage       usage;
        uint32_t    access;

        ResourceDesc()
            : usage(USAGE_DEFAULT)
            , access(0)
        {}
    };

    class IResource : public IDeviceChild
    {
    public:
        virtual const ResourceDesc &
        GetDesc() const = 0;

        virtual ResourceType
        GetType() const = 0;

        virtual
        ~IResource() {}
    };
    SHARED_PTR_TYPEDEFS(IResource);

} // namespace GFW

#endif // __GFW_BASE_RESOURCE_H__
