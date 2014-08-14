#ifndef __GFW_SWAP_CHAIN_H__
#define __GFW_SWAP_CHAIN_H__

#include "gfw/types_fwd.h"

namespace GFW {

    struct SwapChainDesc
    {
    };

    class ISwapChain
    {
    public:
        virtual const SwapChainDesc &
        GetDesc() const = 0;

        virtual void
        Present() = 0;

        virtual ConstIRenderTargetRef
        GetBuffer() const = 0;

        virtual
        ~ISwapChain() {}
    };

} // namespace GFW

#endif // __GFW_SWAP_CHAIN_H__
