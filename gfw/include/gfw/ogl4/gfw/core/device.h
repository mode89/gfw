#ifndef __GFW_CORE_DEVICE_H__
#define __GFW_CORE_DEVICE_H__

#include "common/futex.h"

#include "gfw/base/device.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class Device: public IDevice
    {
    public:
        virtual IContextRef
        CreateContext();

        virtual IShaderRef
        CreateShader(int32_t stage, const void * shaderData);

        virtual IBufferRef
        CreateBuffer(const BufferDesc &, const void * initialData);

        virtual ITextureRef
        CreateTexture(const TextureDesc &, const void * initialData = 0);

        virtual IRenderBufferRef
        CreateRenderBuffer(ITextureIn, const SubResIdx &);

        inline virtual IContextRef
        GetDefaultContext() { return mDefaultContext; }

        inline virtual IRenderBufferRef
        GetDefaultColorBuffer() { return mDefaultColorBuffer; }

        virtual void
        Present();

    public:
        Device(const DeviceParams &);
        ~Device();

        bool
        Initialize();

    private:
        static PLAT_THREAD_LOCAL
        IContext *                  mCurrentContext;

        const DeviceParams          mParams;

        IDrawingContextRef          mDrawingContext;
        RenderingContext            mContextGL;

        Common::Futex               mMutex;
        IContextRef                 mDefaultContext;
        IRenderBufferRef            mDefaultColorBuffer;
    };
    AUTOREF_REFERENCE_DECLARATION(Device);

} // namespace GFW

#endif // __GFW_CORE_DEVICE_H__
