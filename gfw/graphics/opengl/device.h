#ifndef __GFW_GRAPHICS_OPENGL_DEVICE_H__
#define __GFW_GRAPHICS_OPENGL_DEVICE_H__

#include "gfw\graphics\base\device.h"

namespace GFW { namespace OpenGL {

    class Device: public IDevice
    {
    public:
        virtual IContextRef     CreateContext(GFW::Platform::IWindowIn);
        virtual void            Release();

    public:
        static IDeviceRef       CreateInstance();

    public:
        Device();
    };

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_DEVICE_H__
