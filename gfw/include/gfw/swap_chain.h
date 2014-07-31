#ifndef __GFW_SWAP_CHAIN_H__
#define __GFW_SWAP_CHAIN_H__

namespace GFW {

    struct SwapChainDesc
    {
    };

    class ISwapChain
    {
    public:
        virtual
        ~ISwapChain() {}
    };

} // namespace GFW

#endif // __GFW_SWAP_CHAIN_H__
