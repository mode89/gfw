#ifndef __GFW_RUNTIME_OGL4_FACTORY_H__
#define __GFW_RUNTIME_OGL4_FACTORY_H__

#include "gfw/runtime/common/factory.h"

namespace GFW {

    class Factory : public AFactory
    {
    public:
        virtual ISwapChainRef
        CreateSwapChain( const SwapChainDesc &, const WindowHandle & );

        virtual IDeviceRef
        CreateDevice( const DeviceParams &, ISwapChainIn );

        Factory();
        ~Factory();
    };
    SHARED_PTR_TYPEDEFS( Factory );

} // namespace GFW

#endif // __GFW_RUNTIME_OGL4_FACTORY_H__
