#ifndef __GFW_BASE_MESH_H__
#define __GFW_BASE_MESH_H__

#include "gfw/base/types_fwd.h"
#include "gfw/base/device_child.h"

namespace GFW {

    class IMesh
    {
    public:
        virtual void
        Draw() = 0;

        virtual
        ~IMesh() {}
    };
    SHARED_PTR_TYPEDEFS(IMesh);

} // namespace GFW

#endif // __GFW_BASE_MESH_H__
