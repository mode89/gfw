#ifndef __GFW_BASE_FACTORY_H__
#define __GFW_BASE_FACTORY_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    class IFactory : public Common:: ARefCounted
    {
    public:
        virtual
        ~IFactory() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IFactory);

    IFactoryRef
    CreateFactory(IDeviceRef);

} // namespace GFW

#endif // __GFW_BASE_FACTORY_H__
