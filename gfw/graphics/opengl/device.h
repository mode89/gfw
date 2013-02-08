#ifndef __GFW_GRAPHICS_OPENGL_DEVICE_H__
#define __GFW_GRAPHICS_OPENGL_DEVICE_H__

#include "gfw\graphics\base\device.h"
#include "gfw\graphics\opengl\platform.h"

namespace GFW { namespace OpenGL {

    class Device: public IDevice
    {
    public:

        virtual IContextRef         CreateContext(GFW::Platform::IWindowIn);

        virtual IShaderRef          CreateShader(ShaderStage, const void * shaderData);

        virtual IBufferRef          CreateBuffer(const BufferDesc &, const void * initialData);

        virtual ITextureRef         CreateTexture(const TextureDesc &, const void * initialData = 0);

        virtual IRenderBufferRef    CreateColorBuffer(ITextureIn, const SubResIdx &);

    public:
        static IDeviceRef           CreateInstance(Common::IAllocator *);

        inline
        IPlatformRef                GetPlatform()   { return mPlatform; }

    public:
        Device(IPlatformIn, Common::IAllocator *);

    private:
        IPlatformRef                mPlatform;
    };
    AUTOREF_REFERENCE_DECLARATION(Device);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_DEVICE_H__
