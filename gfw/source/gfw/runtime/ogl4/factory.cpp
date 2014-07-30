#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/factory.h"

namespace GFW {

    IFactoryRef CreateFactory()
    {
        return std::make_shared< Factory >();
    }

    Factory::Factory()
    {
    }

    Factory::~Factory()
    {
    }

    IDeviceRef Factory::CreateDevice( const DeviceParams & deviceParams )
    {
        return std::make_shared< Device >( deviceParams );
    }

} // namespace GFW
