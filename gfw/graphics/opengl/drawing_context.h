#ifndef __GFW_GRAPHICS_OPENGL_DRAWING_CONTEXT_H__
#define __GFW_GRAPHICS_OPENGL_DRAWING_CONTEXT_H__

#include "common/autoref.h"
#include "gfw/common/window.h"

namespace GFW { namespace OpenGL {

    typedef void * NativeContext;

    class IDrawingContext : public Common::ARefCounted
    {
    public:

        virtual NativeContext   CreateContext() = 0;

        virtual void            DeleteContext(NativeContext) = 0;

        virtual void            MakeCurrent(NativeContext) = 0;

        virtual NativeContext   GetCurrentContext() = 0;

        virtual bool            SwapBuffers() = 0;

        virtual NativeContext   GetRenderingContext() = 0;

    public:
        virtual ~IDrawingContext() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IDrawingContext);

    IDrawingContextRef CreateDrawingContext(WindowHandle);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_DRAWING_CONTEXT_H__
