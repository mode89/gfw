#ifndef __GFW_GRAPHICS_OPENGL_DEVICE_H__
#define __GFW_GRAPHICS_OPENGL_DEVICE_H__

#include "common/futex.h"
#include "gfw/graphics/base/device.h"
#include "gfw/graphics/opengl/drawing_context.h"

namespace GFW { namespace OpenGL {

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

    public:

        inline
        NativeContext               CreateNativeContext()
                                        { return mDrawingContext->CreateContext(); }

        inline
        void                        DeleteNativeContext(NativeContext nativeContext)
                                        { return mDrawingContext->DeleteContext(nativeContext); }

        inline
        void                        MakeCurrent(NativeContext nativeContext)
                                        { mDrawingContext->MakeCurrent(nativeContext); }

        inline
        NativeContext               GetCurrentContext()
                                        { return mDrawingContext->GetCurrentContext(); }

    public:
        Device(WindowHandle);

    private:
        Common::Futex               mMutex;
        IDrawingContextRef          mDrawingContext;
        NativeContext               mNativeContext;
        IContextRef                 mImmediateContext;
        IRenderBufferRef            mDefaultColorBuffer;
    };
    AUTOREF_REFERENCE_DECLARATION(Device);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_DEVICE_H__
