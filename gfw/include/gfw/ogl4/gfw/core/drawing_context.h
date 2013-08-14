#ifndef __GFW_OGL_DRAWING_CONTEXT_H__
#define __GFW_OGL_DRAWING_CONTEXT_H__

#include "common/autoref.h"

#include "gfw/base/window.h"

#include "gfw/core/types_fwd.h"

namespace GFW {

    class IDrawingContext : public Common::ARefCounted
    {
    public:
        virtual RenderingContext    CreateContext() = 0;
        virtual void                DeleteContext(RenderingContext) = 0;
        virtual void                MakeCurrent(RenderingContext) = 0;
        virtual RenderingContext    GetCurrentContext() = 0;
        virtual void                SwapBuffers() = 0;

        virtual                     ~IDrawingContext()  {}
    };
    AUTOREF_REFERENCE_DECLARATION(IDrawingContext);

    IDrawingContextRef CreateDrawingContext(WindowHandle);

} // namespace GFW

#endif // __GFW_OGL_DRAWING_CONTEXT_H__
