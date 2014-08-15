#ifndef __GFW_RUNTIME_CORE_DEVICE_H__
#define __GFW_RUNTIME_CORE_DEVICE_H__

#include "cmn/platform.h"

#include "gfw/context.h"
#include "gfw/device.h"
#include "gfw/render_target.h"
#include "gfw/runtime/ogl4/types_fwd.h"

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
        CreateBuffer( const BufferDesc &, const SubResourceData * initialData = nullptr );

        virtual ITextureRef
        CreateTexture( const TextureDesc &, const SubResourceData * initialData = nullptr );

        virtual IRenderTargetRef
        CreateRenderTarget( ConstITextureIn, const RenderTargetDesc & );

        virtual IContextRef
        GetCurrentContext() const;

        virtual IContextRef
        GetDefaultContext() const { return mDefaultContext; }

    public:
        Device( const DeviceParams &, ISwapChainIn );
        ~Device();

        // shared_from_this() cannot be called from constructor
        void
        InitializeChildren();

        void
        LockContext( IContextIn );

        void
        UnlockContext( IContextIn );

        uint32_t
        GetResolveShaderProgram() const { return mResolveShaderProgram; }

        Device( const Device & );
        Device & operator= ( const Device & );

    private:
        static CMN_THREAD_LOCAL
        IContext *                  mCurrentContext;

        const DeviceParams          mParams;

        SwapChainRef                mSwapChain;

        NativeContextRef            mNativeContext;

        std::mutex                  mMutex;
        IContextRef                 mDefaultContext;

        IRenderTargetRef            mDefaultRenderTarget;

        uint32_t                    mResolveShaderProgram;
    };
    SHARED_PTR_TYPEDEFS( Device );

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_DEVICE_H__
