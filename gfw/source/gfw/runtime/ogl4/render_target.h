#ifndef __GFW_RUNTIME_CORE_RENDER_TARGET_H__
#define __GFW_RUNTIME_CORE_RENDER_TARGET_H__

#include "gfw/render_target.h"
#include "gfw/types_fwd.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/ogl4/types_fwd.h"

namespace GFW {

    class RenderTarget : public ADeviceChild<IRenderTarget>
    {
    public:
        virtual const RenderTargetDesc &
        GetDesc() const { return mDesc; }

        virtual ConstITextureRef
        GetTexture() const { return std::static_pointer_cast<const ITexture>( mTexture ); }

    public:
        RenderTarget( ConstITextureIn, const RenderTargetDesc &, DeviceIn );
        ~RenderTarget();

    private:
        RenderTargetDesc    mDesc;
        ConstTextureRef     mTexture;
    };
    SHARED_PTR_TYPEDEFS(RenderTarget);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_RENDER_TARGET_H__
