#ifndef __GFW_RUNTIME_CORE_TYPES_FWD_H__
#define __GFW_RUNTIME_CORE_TYPES_FWD_H__

#include "common/shared_ptr_typedefs.h"

#include <memory>

namespace GFW {

    SHARED_PTR_FORWARD_TYPEDEFS(IDrawingContext);

    SHARED_PTR_FORWARD_TYPEDEFS(Buffer);
    SHARED_PTR_FORWARD_TYPEDEFS(Context);
    SHARED_PTR_FORWARD_TYPEDEFS(Device);
    SHARED_PTR_FORWARD_TYPEDEFS(InputLayout);
    SHARED_PTR_FORWARD_TYPEDEFS(RenderTarget);
    SHARED_PTR_FORWARD_TYPEDEFS(Shader);
    SHARED_PTR_FORWARD_TYPEDEFS(ShaderReflection);
    SHARED_PTR_FORWARD_TYPEDEFS(Texture);

    typedef void * RenderingContext;

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_TYPES_FWD_H__
