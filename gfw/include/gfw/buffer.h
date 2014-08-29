#ifndef __GFW_BASE_BUFFER_H__
#define __GFW_BASE_BUFFER_H__

#include "gfw/resource.h"
#include "gfw/types_fwd.h"

namespace GFW {

    struct BufferDesc : public ResourceDesc
    {
        uint32_t    size;

        BufferDesc()
            : size(0)
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
