#include "common/trace.h"
#include "gfw/base/device.h"
#include "gfw/common/factory.h"

namespace GFW {

    IFactoryRef CreateFactory(IDeviceRef device)
    {
        Factory * factory = new Factory(device);
        if (!factory->Initialize())
        {
            TRACE_FAIL_MSG("Failed to initialize a factory");
            delete factory;
            return NULL;
        }
        return factory;
    }

    Factory::Factory(IDeviceRef device)
        : mDevice(device)
    {

    }

    Factory::~Factory()
    {

    }

    bool Factory::Initialize()
    {
        return true;
    }

} // namespace GFW
