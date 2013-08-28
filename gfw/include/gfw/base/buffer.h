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
        GetDesc() = 0;
    };
    AUTOREF_REFERENCE_DECLARATION(IBuffer);

} // namespace GFW

#endif // _GFW_BASE_BUFFER_H__
