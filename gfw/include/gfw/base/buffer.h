#ifndef __GFW_BASE_BUFFER_H__
#define __GFW_BASE_BUFFER_H__

#include "common/autoref.h"
#include "gfw/base/resource.h"

namespace GFW {

    struct BufferDesc
    {
        BufferType  type;
        Usage       usage;
        uint32_t    size;

        BufferDesc()
            : type(BUFFER_UNKNOWN)
            , usage(USAGE_UNKNOWN)
            , size(0)
        {}
    };

    class IBuffer: public Common::ARefCounted
    {

    };
    AUTOREF_REFERENCE_DECLARATION(IBuffer);

} // namespace GFW

#endif // _GFW_BASE_BUFFER_H__
