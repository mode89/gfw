#ifndef __GFW_COMMON_FACTORY_H__
#define __GFW_COMMON_FACTORY_H__

#include "gfw/base/factory.h"
#include "gfw/base/types_fwd.h"

namespace GFW {

    class Factory : public IFactory
    {
    public:
        virtual IEffectRef
        CreateEffect(const char * fileName);

    public:
        Factory(IDeviceRef);
        ~Factory();

        bool
        Initialize();

    private:
        IDeviceRef  mDevice;
    };
    AUTOREF_REFERENCE_DECLARATION(IFactory);

} // namespace GFW

#endif // __GFW_COMMON_FACTORY_H__
