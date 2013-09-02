#ifndef __GFW_BASE_MESH_H__
#define __GFW_BASE_MESH_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    class IMesh : public IDeviceChild
    {
    public:
        virtual void
        Draw() = 0;

        virtual
        ~IMesh() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IMesh);

} // namespace GFW

#endif // __GFW_BASE_MESH_H__
