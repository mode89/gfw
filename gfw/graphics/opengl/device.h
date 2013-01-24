#ifndef __GFW_GRAPHICS_OPENGL_DEVICE_H__
#define __GFW_GRAPHICS_OPENGL_DEVICE_H__

#include "gfw\graphics\common\device.h"

namespace GFW { namespace OpenGL {

    class Device: public ADevice
    {
    public:
        virtual IContextRef     CreateContext(Platform::IWindowIn);
        virtual void            Release();

    public:
        static IDeviceRef       CreateInstance(Common::IAllocator *);

    public:
        Device(Common::IAllocator *);
    };

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_DEVICE_H__
