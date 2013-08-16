#ifndef __GFW_BASE_BUFFER_H__
#define __GFW_BASE_BUFFER_H__

#include "gfw/base/resource.h"
#include "gfw/base/types_fwd.h"

namespace GFW {

    enum BufferType
    {
        BUFFER_UNKNOWN = 0,
        BUFFER_VERTEX,
        BUFFER_INDEX,
        BUFFER_PIXEL
    };

    struct BufferDesc
    {
        BufferType  type;
        Usage       usage;
        uint32_t    access;     // See CpuAccessFlags
        uint32_t    size;

        BufferDesc()
            : type(BUFFER_UNKNOWN)
            , usage(USAGE_DEFAULT)
            , access(0)
            , size(0)
        {}
    };

    class IBuffer: public IResource
    {
    public:
        virtual const BufferDesc &
        GetDesc() = 0;
    };
    AUTOREF_REFERENCE_DECLARATION(IBuffer);

} // namespace GFW

#endif // _GFW_BASE_BUFFER_H__
