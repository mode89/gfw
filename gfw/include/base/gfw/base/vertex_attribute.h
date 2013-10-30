#ifndef __GFW_BASE_VERTEX_ATTRIBUTE_H__
#define __GFW_BASE_VERTEX_ATTRIBUTE_H__

#include "gfw/base/format.h"
#include "gfw/shared/semantic.h"

namespace GFW {

    struct VertexAttribute
    {
        Semantic        semantic;
        Format          format;
        uint32_t        stride;
        uint32_t        offset;
        uint32_t        bufSlot;

        VertexAttribute()
            : semantic(SEMANTIC_UNKNOWN)
            , format(FORMAT_UNKNOWN)
            , stride(0)
            , offset(0)
            , bufSlot(0)
        {}
    };

} // namespace GFW

#endif // __GFW_BASE_VERTEX_ATTRIBUTE_H__
