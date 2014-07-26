#ifndef __GFW_RUNTIME_CORE_DEVICE_H__
#define __GFW_RUNTIME_CORE_DEVICE_H__

#include "cmn/platform.h"

#include "gfw/base/context.h"
#include "gfw/base/device.h"
#include "gfw/base/render_target.h"
#include "gfw/runtime/core/types_fwd.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4265 )
#include <mutex>
CMN_WARNING_POP

namespace GFW {

    class Device: public IDevice, public std::enable_shared_from_this< Device >
    {
    public:
        virtual IContextRef
        CreateContext();

        virtual IShaderRef
        CreateShader( ShaderStage stage, const void * shaderBinary );

        virtual IInputLayoutRef
        CreateInputLayout( uint32_t attrCnt, VertexAttribute[], ConstIShaderIn vertexShader );

        virtual IBufferRef
        CreateBuffer( const BufferDesc &, const void * initialData );

        virtual ITextureRef
        CreateTexture( const TextureDesc &, const void * initialData = 0 );

        virtual IRenderTargetRef
        CreateRenderTarget( ConstITextureIn, const RenderTargetDesc & );

        virtual IContextRef
        GetCurrentContext() const { return IContextRef( mCurrentContext ); }

        virtual IContextRef
        GetDefaultContext() const { return mDefaultContext; }

        virtual ConstIRenderTargetRef
        GetDefaultRenderTarget() const { return mDefaultRenderTarget; }

        virtual void
        Present();

    public:
        Device( const DeviceParams & );
        ~Device();

        void
        InitializeSwapChain();

        void
        LockContext(IContextRef);

        void
        UnlockContext(IContextRef);

        Device( const Device & );
        Device & operator= ( const Device & );

    private:
        static CMN_THREAD_LOCAL
        IContext*                   mCurrentContext;

        const DeviceParams          mParams;

        IDrawingContextRef          mDrawingContext;
        RenderingContext            mContextGL;

        std::mutex                  mMutex;
        IContextRef                 mDefaultContext;

        IRenderTargetRef            mDefaultRenderTarget;
        uint32_t                    mResolveFramebuffer;
    };
    SHARED_PTR_TYPEDEFS( Device );

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_DEVICE_H__
