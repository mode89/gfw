#ifndef __GFW_RUNTIME_CORE_DEVICE_H__
#define __GFW_RUNTIME_CORE_DEVICE_H__

#include "common/mutex.h"
#include "common/platform.h"
#include "common/string_table.h"

#include "gfw/base/context.h"
#include "gfw/base/device.h"
#include "gfw/base/render_target.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class Device: public IDevice
    {
    public:
        virtual IContextRef
        CreateContext();

        virtual IShaderRef
        CreateShader( ShaderStage stage, ShaderBinaryRef );

        virtual IInputLayoutRef
        CreateInputLayout(uint32_t attrCnt, VertexAttribute[], IShaderIn vertexShader);

        virtual IBufferRef
        CreateBuffer(const BufferDesc &, const void * initialData);

        virtual ITextureRef
        CreateTexture(const TextureDesc &, const void * initialData = 0);

        virtual IRenderTargetRef
        CreateRenderTarget(ITextureIn, const RenderTargetDesc &);

        inline virtual IContextRef
        GetCurrentContext() { return mCurrentContext; }

        inline virtual IContextRef
        GetDefaultContext() { return mDefaultContext; }

        inline virtual IRenderTargetRef
        GetDefaultRenderTarget() { return mDefaultRenderTarget; }

        virtual void
        Present();

    public:
        Device(const DeviceParams &);
        ~Device();

        bool
        Initialize();

        void
        InitializeSwapChain();

        void
        LockContext(IContextRef);

        void
        UnlockContext(IContextRef);

        inline Common::StringTable &
        GetStringTable() { return mStringTable; }

    private:
        static PLAT_THREAD_LOCAL
        IContext*                   mCurrentContext;

        const DeviceParams          mParams;

        IDrawingContextRef          mDrawingContext;
        RenderingContext            mContextGL;

        Common::Mutex               mMutex;
        IContextRef                 mDefaultContext;

        IRenderTargetRef            mDefaultRenderTarget;
        uint32_t                    mResolveFramebuffer;

        Common::StringTable         mStringTable;
    };
    AUTOREF_REFERENCE_DECLARATION(Device);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_DEVICE_H__
