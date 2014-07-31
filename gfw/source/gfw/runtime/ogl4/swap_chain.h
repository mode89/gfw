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
        virtual void
        Present();

    public:
        NativeContextRef
        CreateContext();

        void
        MakeCurrent( NativeContext * );
        
        NativeContext *
        GetCurrentContext();

        void
        ShareLists( NativeContext *, NativeContext * );

    public:
        SwapChain( const SwapChainDesc &, const WindowHandle & );
        ~SwapChain();

    private:
        WindowHandle                mWindow;
        std::shared_ptr< void >     mDC;
    };
    SHARED_PTR_TYPEDEFS( SwapChain );

} // namespace GFW

#endif // __GFW_RUNTIME_OGL4_SWAP_CHAIN_H__
