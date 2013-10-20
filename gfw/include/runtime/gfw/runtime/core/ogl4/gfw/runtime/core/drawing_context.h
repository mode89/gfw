#ifndef __GFW_CORE_DRAWING_CONTEXT_H__
#define __GFW_CORE_DRAWING_CONTEXT_H__

#include "gfw/base/types_fwd.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    typedef void * RenderingContext;

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

#endif // __GFW_CORE_DRAWING_CONTEXT_H__
