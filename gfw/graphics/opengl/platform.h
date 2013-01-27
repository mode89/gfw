#ifndef __GFW_GRAPHICS_OPENGL_PLATFORM_H__
#define __GFW_GRAPHICS_OPENGL_PLATFORM_H__

#include "common\autoref.h"
#include "common\allocator.h"
#include "gfw\platform\base\window.h"

namespace GFW { namespace OpenGL {

    class IPlatform: public Common::AutoRefObject
    {
    public:
        virtual uint32_t                Init() = 0;

        virtual Platform::IWindowRef    CreateOpenglWindow(Platform::IWindowIn) = 0;

        virtual void                    Release() = 0;
    };
    AUTOREF_REFERENCE_DECLARATION(IPlatform);

    IPlatformRef CreatePlatform(Common::IAllocator *);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_PLATFORM_H__
