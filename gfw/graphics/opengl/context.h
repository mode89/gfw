#ifndef __GFW_GRAPHICS_OPENGL_CONTEXT_H__
#define __GFW_GRAPHICS_OPENGL_CONTEXT_H__

#include "gfw\graphics\base\context.h"
#include "gfw\platform\base\window.h"

namespace GFW { namespace OpenGL {

    class Context: public IContext
    {
    public:
        virtual void    Clear(ClearParams &);
        virtual void    Present();
        virtual void    Release();

    public:
        Context(Platform::IWindowIn);
    };

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_CONTEXT_H__
