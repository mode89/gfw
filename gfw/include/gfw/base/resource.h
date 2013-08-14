#ifndef __GFW_BASE_RESOURCE_H__
#define __GFW_BASE_RESOURCE_H__

#include "common/autoref.h"
#include "gfw/base/context.h"

namespace GFW {

    enum Usage
    {
        USAGE_UNKNOWN = 0,
        USAGE_STREAM_DRAW,
        USAGE_STREAM_READ,
        USAGE_STREAM_COPY,
        USAGE_STATIC_DRAW,
        USAGE_STATIC_READ,
        USAGE_STATIC_COPY,
        USAGE_DYNAMIC_DRAW,
        USAGE_DYNAMIC_READ,
        USAGE_DYNAMIC_COPY
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

    class IResource : public Common::ARefCounted
    {
    public:
        virtual void *
        Map(IContextRef, uint32_t mapFlags) = 0;

        virtual void
        Unmap() = 0;

        virtual
        ~IResource() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IResource);

} // namespace GFW

#endif // __GFW_BASE_RESOURCE_H__
