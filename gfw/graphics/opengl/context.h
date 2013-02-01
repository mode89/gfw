#ifndef __GFW_GRAPHICS_OPENGL_CONTEXT_H__
#define __GFW_GRAPHICS_OPENGL_CONTEXT_H__

#include "gfw\graphics\common\context.h"
#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\window.h"

namespace GFW { namespace OpenGL {

    class Context: public AContext
    {
    public:
        virtual void            Clear(ClearParams &);

        virtual void            Present();

    public:
        Context(Platform::IWindowIn, DeviceIn, Common::IAllocator *);
        ~Context();

    private:
        DeviceRef               mDevice;
        OpenglWindowRef         mWindow;

    protected:
        virtual void *          GetImplThis()   { return this; }
    };

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_CONTEXT_H__
