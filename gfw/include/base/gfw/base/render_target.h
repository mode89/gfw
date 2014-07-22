#ifndef __GFW_GRAPHICS_BASE_RENDER_TARGET_H__
#define __GFW_GRAPHICS_BASE_RENDER_TARGET_H__

#include "gfw/base/device_child.h"
#include "gfw/base/format.h"
#include "gfw/base/types_fwd.h"

namespace GFW {

    struct RenderTargetDesc
    {
        Format      format;
        uint32_t    resourceIndex;

        RenderTargetDesc()
            : format(FORMAT_UNKNOWN)
            , resourceIndex(0)
        {}
    };

    class IRenderTarget: public IDeviceChild
    {
    public:
        virtual const RenderTargetDesc &
        GetDesc() const = 0;

        virtual ConstITextureRef
        GetTexture() const = 0;

    public:
        virtual
        ~IRenderTarget() {}
    };
    SHARED_PTR_TYPEDEFS(IRenderTarget);

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_RENDER_TARGET_H__
