#ifndef __GFW_GRAPHICS_OPENGL_DEVICE_H__
#define __GFW_GRAPHICS_OPENGL_DEVICE_H__

#include "gfw\graphics\common\device.h"
#include "gfw\graphics\opengl\platform.h"

namespace GFW { namespace OpenGL {

    class Device: public ADevice
    {
    public:
        virtual IContextRef     CreateContext(Platform::IWindowIn);

    public:
        static IDeviceRef       CreateInstance(Common::IAllocator *);

        inline
        IPlatformRef            GetPlatform()   { return mPlatform; }

    public:
        Device(IPlatformIn, Common::IAllocator *);

    private:
        IPlatformRef            mPlatform;

    protected:
        virtual void *          GetImplThis()   { return this; }
    };
    AUTOREF_REFERENCE_DECLARATION(Device);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_DEVICE_H__
