#ifndef __GFW_OGL_DEVICE_H__
#define __GFW_OGL_DEVICE_H__

#include "gfw/base/device.h"

#include "gfw/ogl/drawing_context.h"

namespace GFW {

    class Device: public IDevice
    {
    public:

        virtual IContextRef     CreateContext();

        virtual IShaderRef      CreateShader(ShaderStage, const void * shaderData);

        virtual IBufferRef      CreateBuffer(const BufferDesc &, const void * initialData);

        virtual bool            Present();

    public:

        Device(const DeviceParams &);
        ~Device();

        bool                    Initialize();

    private:

        const DeviceParams      mParams;

        IDrawingContextRef      mDrawingContext;
        RenderingContext        mRenderingContext;

    };
    AUTOREF_REFERENCE_DECLARATION(Device);

} // namespace GFW

#endif // __GFW_OGL_DEVICE_H__
