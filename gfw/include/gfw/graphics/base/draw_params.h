#ifndef __GFW_GRAPHICS_BASE_DRAW_PARAMS_H__
#define __GFW_GRAPHICS_BASE_DRAW_PARAMS_H__

#include "common/typedefs.h"
#include "gfw/graphics/base/primitive.h"

namespace GFW {

    struct DrawParams
    {
        Primitive   primTop;
        uint32_t    vertexStart;
        uint32_t    vertexCount;

        DrawParams()
            : primTop(PRIM_UNKNOWN)
            , vertexStart(0)
            , vertexCount(0)
        {}
    };

    struct DrawIndexedParams
    {
        Primitive   primTop;
        Type        indexType;
        uint32_t    indexStart;
        uint32_t    indexCount;

        DrawIndexedParams()
            : primTop(PRIM_UNKNOWN)
            , indexType(TYPE_UNKNOWN)
            , indexStart(0)
            , indexCount(0)
        {}
    };

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_DRAW_PARAMS_H__
