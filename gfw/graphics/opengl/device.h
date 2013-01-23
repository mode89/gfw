#ifndef __GFW_DEVICE_OPENGL_DEVICE_H__
#define __GFW_DEVICE_OPENGL_DEVICE_H__

#include "gfw\graphics\base\device.h"

namespace GFW { namespace OpenGL {

    class Device: public IDevice
    {
    public:
        virtual IContextRef     CreateContext(GFW::Platform::IWindowIn);
        virtual void            Release();
    };

}} // namespace GFW::OpenGL

#endif // __GFW_DEVICE_OPENGL_DEVICE_H__
