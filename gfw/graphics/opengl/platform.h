#ifndef __GFW_GRAPHICS_OPENGL_PLATFORN_H__
#define __GFW_GRAPHICS_OPENGL_PLATFORM_H__

#include "common\typedefs.h"
#include "common\platform.h"
#include "gfw\platform\base\window.h"

#if PLATFORM_WIN32
#include <windows.h>
#else
#error Unrecognized platform
#endif

namespace GFW { namespace OpenGL {

#if PLATFORM_WIN32

    struct ContextDescPlat
    {
        HGLRC hRC;
    };

#else
#error Urecognized platform
#endif

    uint32_t LoadFunctions();

    uint32_t InitPlatformContext(Platform::IWindowIn, ContextDescPlat &);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_PLATFORM_H__
