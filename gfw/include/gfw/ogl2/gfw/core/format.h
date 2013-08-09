#ifndef __GFW_OGL_FORMAT_H__
#define __GFW_OGL_FORMAT_H__

#include "gfw/base/format.h"
#include "gfw/base/primitive.h"

namespace GFW {

    uint32_t    GetOGLType(Type);

    uint32_t    GetOGLDrawMode(Primitive);

} // namespace GFW

#endif // __GFW_OGL_FORMAT_H__
