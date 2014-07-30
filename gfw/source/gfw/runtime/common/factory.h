#ifndef __GFW_RUNTIME_COMMON_FACTORY_H__
#define __GFW_RUNTIME_COMMON_FACTORY_H__

#include "gfw/factory.h"
#include "gfw/types_fwd.h"

namespace GFW {

    class AFactory : public IFactory
    {
    public:
        virtual IEffectRef
        CreateEffect( IDeviceIn, const char * fileName );

        virtual IMeshBuilderRef
        CreateMeshBuilder();

    public:
        AFactory();
        ~AFactory();
    };

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_FACTORY_H__
