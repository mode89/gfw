#ifndef __GFW_SWAP_CHAIN_H__
#define __GFW_SWAP_CHAIN_H__

#include "gfw/format.h"
#include "gfw/types_fwd.h"

namespace GFW {

    struct SwapChainDesc
    {
        uint32_t    width;
        uint32_t    height;
        Format      format;

        SwapChainDesc()
            : width( 0 )
            , height( 0 )
            , format( FORMAT_UNKNOWN )
        {}
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
