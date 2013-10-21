#ifndef __GFW_RUNTIME_CORE_FORMAT_H__
#define __GFW_RUNTIME_CORE_FORMAT_H__

#include "gfw/base/format.h"
#include "gfw/base/primitive.h"

namespace GFW {

    uint32_t    GetOGLType(Type);

    uint32_t    GetOGLType(Format);

    uint32_t    GetOGLDrawMode(Primitive);

    uint32_t    GetOGLInternalFormat(Format);

    uint32_t    GetOGLFormat(Format);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_FORMAT_H__
