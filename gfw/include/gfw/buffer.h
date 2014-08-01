#ifndef __GFW_BASE_BUFFER_H__
#define __GFW_BASE_BUFFER_H__

#include "gfw/resource.h"
#include "gfw/types_fwd.h"

namespace GFW {

    enum BufferType
    {
        BUFFER_UNKNOWN = 0,
        BUFFER_VERTEX,
        BUFFER_INDEX,
        BUFFER_PIXEL
    };

    struct BufferDesc : public ResourceDesc
    {
        BufferType  type;
        uint32_t    size;

        BufferDesc()
            : type(BUFFER_UNKNOWN)
            , size(0)
        {}
    };

    class IBuffer: public IResource
    {
    public:
        virtual const BufferDesc &
        GetDesc() const = 0;
    };
    SHARED_PTR_TYPEDEFS(IBuffer);

} // namespace GFW

#endif // _GFW_BASE_BUFFER_H__
