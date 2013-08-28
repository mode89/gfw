#ifndef __GFW_BASE_RESOURCE_H__
#define __GFW_BASE_RESOURCE_H__

#include "gfw/base/types_fwd.h"
#include "gfw/base/device_child.h"

namespace GFW {

    enum Usage
    {
        USAGE_DEFAULT = 0,      // Only read/write access by GPU is allowed (draw, copy to/from)
        USAGE_STATIC,           // Only read access by GPU is allowed (draw, copy from)
        USAGE_DYNAMIC,          // Can only be read by GPU (draw, copy from) and written by CPU (update, map write)
        USAGE_STAGING           // Can be read/written by GPU (copy to/from) and CPU (update, map read/write)
    };

    enum CpuAccessFlags
    {
        CPU_ACCESS_READ  = (1 << 0),    // Only dynamic or staging resources
        CPU_ACCESS_WRITE = (1 << 1)     // Only staging resources
    };

    struct SubResIdx
    {
        uint32_t    mip;
        uint32_t    slice;

        SubResIdx(uint32_t m, uint32_t s)
            : mip(m)
            , slice(s)
        {}
    };

    enum MapFlags
    {
        MAP_FLAG_READ  = (1 << 0),
        MAP_FLAG_WRITE = (1 << 1)
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
        virtual void *
        Map(uint32_t mapFlags) = 0;

        virtual void
        Unmap() = 0;

        virtual void
        UpdateSubresource(const void * data, uint32_t subResourceIndex = 0) = 0;

        virtual
        ~IResource() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IResource);

} // namespace GFW

#endif // __GFW_BASE_RESOURCE_H__
