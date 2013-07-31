#ifndef __GFW_BASE_RESOURCE_H__
#define __GFW_BASE_RESOURCE_H__

namespace GFW {

    enum BufferType
    {
        BUFFER_UNKNOWN = 0,
        BUFFER_VERTEX,
        BUFFER_INDEX,
        BUFFER_PIXEL
    };

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

} // namespace GFW

#endif // __GFW_BASE_RESOURCE_H__