#ifndef __GFW_RUNTIME_OGL4_SWAP_CHAIN_H__
#define __GFW_RUNTIME_OGL4_SWAP_CHAIN_H__

#include "gfw/swap_chain.h"
#include "gfw/types_fwd.h"
#include "gfw/runtime/ogl4/types_fwd.h"
#include <windows.h>

namespace GFW {

    class SwapChain : public ISwapChain
    {
    public:
        virtual const SwapChainDesc &
        GetDesc() const { return mDesc; }

        virtual void
        Present();

        virtual ConstIRenderTargetRef
        GetBuffer() const;

    public:
        NativeContextRef
        CreateContext();

        void
        MakeCurrent( NativeContext * );
        
        NativeContext *
        GetCurrentContext();

        void
        ShareLists( NativeContext *, NativeContext * );

        NativeContextRef
        GetDefaultNativeContext() const { return mNativeContext; }

    public:
        SwapChain( const SwapChainDesc &, WindowHandleIn );
        ~SwapChain();

    private:
        SwapChainDesc               mDesc;
        WindowHandleRef             mWindow;
        uint32_t                    mWindowWidth;
        uint32_t                    mWindowHeight;
        std::shared_ptr< void >     mDC;
        uint32_t                    mResolveFramebuffer;
        NativeContextRef            mNativeContext;
        RenderTargetRef             mRenderTarget;
    };
    SHARED_PTR_TYPEDEFS( SwapChain );

} // namespace GFW

#endif // __GFW_RUNTIME_OGL4_SWAP_CHAIN_H__
