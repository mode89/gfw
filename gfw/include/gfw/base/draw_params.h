#ifndef __GFW_BASE_DRAW_PARAMS_H__
#define __GFW_BASE_DRAW_PARAMS_H__

#include "common/typedefs.h"
#include "gfw/base/primitive.h"

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

} // namespace GFW

#endif // __GFW_BASE_DRAW_PARAMS_H__
