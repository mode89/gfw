#ifndef __GFW_GRAPHICS_BASE_VERTEX_ATTRIBUTE_H__
#define __GFW_GRAPHICS_BASE_VERTEX_ATTRIBUTE_H__

#include "common/typedefs.h"
#include "gfw/graphics/base/format.h"

namespace GFW {

    struct VertexAttribute
    {
        const char *    name;
        Format          format;
        uint32_t        stride;
        uint32_t        offset;
        uint32_t        bufSlot;
    };

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_VERTEX_ATTRIBUTE_H__
