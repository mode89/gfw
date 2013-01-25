#ifndef __GFW_GRAPHICS_OPENGL_PLATFORN_H__
#define __GFW_GRAPHICS_OPENGL_PLATFORM_H__

#include "common\typedefs.h"
#include "common\platform.h"
#include "gfw\platform\base\window.h"

namespace GFW { namespace OpenGL {

    uint32_t LoadFunctions();

    uint32_t InitPlatformContext(Platform::IWindowIn);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_PLATFORM_H__
