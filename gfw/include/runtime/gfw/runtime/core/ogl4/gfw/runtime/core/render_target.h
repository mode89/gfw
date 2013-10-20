#ifndef __GFW_CORE_RENDER_TARGET_H__
#define __GFW_CORE_RENDER_TARGET_H__

#include "gfw/base/render_target.h"
#include "gfw/base/types_fwd.h"
#include "gfw/common/device_child.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class RenderTarget : public ADeviceChild<IRenderTarget>
    {
    public:
        virtual const RenderTargetDesc &
        GetDesc() { return mDesc; }

        virtual ITextureRef
        GetTexture() { return mTexture; }

    public:
        RenderTarget(ITextureIn, const RenderTargetDesc &, IDeviceIn);
        ~RenderTarget();

    private:
        RenderTargetDesc    mDesc;
        TextureRef          mTexture;
    };
    AUTOREF_REFERENCE_DECLARATION(RenderTarget);

} // namespace GFW

#endif // __GFW_CORE_RENDER_TARGET_H__
