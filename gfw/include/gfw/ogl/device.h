#ifndef __GFW_OGL_DEVICE_H__
#define __GFW_OGL_DEVICE_H__

#include "common/futex.h"
#include "gfw/graphics/base/device.h"
#include "gfw/graphics/opengl/drawing_context.h"

#include "gfw/ogl/drawing_context.h"

namespace GFW {

    class Device: public IDevice
    {
    public:

        virtual IContextRef         CreateContext();

        virtual IShaderRef          CreateShader(ShaderStage, const void * shaderData);

        virtual IBufferRef          CreateBuffer(const BufferDesc &, const void * initialData);

        virtual ITextureRef         CreateTexture(const TextureDesc &, const void * initialData = 0);

        virtual IRenderBufferRef    CreateRenderBuffer(ITextureIn, const SubResIdx &);

        virtual bool                Present(bool clearState = true);

        inline
        virtual IContextRef         GetDefaultContext() { return mImmediateContext; }

        inline
        virtual IRenderBufferRef    GetDefaultColorBuffer() { return mDefaultColorBuffer; }

        virtual bool                Present();

    public:

        Device(const DeviceParams &);
        ~Device();

        bool                        Initialize();

    private:

        const DeviceParams          mParams;

        IDrawingContextRef          mDrawingContext;
        RenderingContext            mRenderingContext;

        Common::Futex               mMutex;
        IDrawingContextRef          mDrawingContext;
        NativeContext               mNativeContext;
        IContextRef                 mImmediateContext;
        IRenderBufferRef            mDefaultColorBuffer;
    };
    AUTOREF_REFERENCE_DECLARATION(Device);

} // namespace GFW

#endif // __GFW_OGL_DEVICE_H__
