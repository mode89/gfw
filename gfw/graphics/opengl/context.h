#ifndef __GFW_GRAPHICS_OPENGL_CONTEXT_H__
#define __GFW_GRAPHICS_OPENGL_CONTEXT_H__

#include "gfw\graphics\common\context.h"
#include "gfw\graphics\opengl\device.h"
#include "gfw\platform\base\window.h"

namespace GFW { namespace OpenGL {

    class Context: public AContext
    {
    public:
        virtual void            Clear(ClearParams &);

        virtual void            Present();

        virtual void            Release();

    public:
        Context(Platform::IWindowIn, DeviceIn, Common::IAllocator *);

    private:
        DeviceRef               mDevice;
        Platform::IWindowRef    mWindow;
    };

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_CONTEXT_H__
