#ifndef __GFW_CORE_DEVICE_H__
#define __GFW_CORE_DEVICE_H__

#include "common/mutex.h"
#include "common/string_table.h"

#include "gfw/base/device.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class Device: public IDevice
    {
    public:
        virtual IContextRef
        CreateContext();

        virtual IShaderRef
        CreateShader(ShaderStage stage, const void * shaderData);

        virtual IInputLayoutRef
        CreateInputLayout(uint32_t attrCnt, VertexAttribute[], IShaderIn vertexShader);

        virtual IBufferRef
        CreateBuffer(const BufferDesc &, const void * initialData);

        virtual ITextureRef
        CreateTexture(const TextureDesc &, const void * initialData = 0);

        virtual IRenderBufferRef
        CreateRenderBuffer(ITextureIn, const SubResIdx &);

        inline virtual IContextRef
        GetCurrentContext() { return mCurrentContext; }

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
        IRenderBufferRef            mDefaultColorBuffer;

        Common::StringTable         mStringTable;
    };
    AUTOREF_REFERENCE_DECLARATION(Device);

} // namespace GFW

#endif // __GFW_CORE_DEVICE_H__
