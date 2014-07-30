#ifndef __GFW_BASE_FACTORY_H__
#define __GFW_BASE_FACTORY_H__

#include "gfw/types_fwd.h"

namespace GFW {

    class IFactory
    {
    public:
        virtual IEffectRef
        CreateEffect(const char * fileName) = 0;

        virtual IMeshBuilderRef
        CreateMeshBuilder() = 0;

        virtual
        ~IFactory() {}
    };
    SHARED_PTR_TYPEDEFS(IFactory);

    IFactoryRef
    CreateFactory( IDeviceIn );

} // namespace GFW

#endif // __GFW_BASE_FACTORY_H__
