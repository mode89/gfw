#ifndef __GFW_RUNTIME_COMMON_FACTORY_H__
#define __GFW_RUNTIME_COMMON_FACTORY_H__

#include "gfw/factory.h"
#include "gfw/types_fwd.h"

namespace GFW {

    class Factory : public IFactory
    {
    public:
        virtual IEffectRef
        CreateEffect(const char * fileName);

        virtual IMeshBuilderRef
        CreateMeshBuilder();

    public:
        Factory( IDeviceIn );
        ~Factory();

    private:
        std::weak_ptr<IDevice>  mDevice;
    };
    SHARED_PTR_TYPEDEFS(IFactory);

} // namespace GFW

#endif // __GFW_RUNTIME_COMMON_FACTORY_H__
