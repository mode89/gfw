#ifndef __GFW_CORE_TYPES_FWD_H__
#define __GFW_CORE_TYPES_FWD_H__

#include "common/autoref.h"

namespace GFW {

    AUTOREF_FORWARD_DECLARATION(IDrawingContext);

    AUTOREF_FORWARD_DECLARATION(Buffer);
    AUTOREF_FORWARD_DECLARATION(Context);
    AUTOREF_FORWARD_DECLARATION(Device);
    AUTOREF_FORWARD_DECLARATION(InputLayout);
    AUTOREF_FORWARD_DECLARATION(Shader);
    AUTOREF_FORWARD_DECLARATION(Texture);

    typedef void * RenderingContext;

} // namespace GFW

#endif // __GFW_CORE_TYPES_FWD_H__
