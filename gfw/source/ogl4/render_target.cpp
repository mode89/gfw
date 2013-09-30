#include "gfw/core/render_target.h"
#include "gfw/core/texture.h"
#include "gfw/common/device_child.inl"

namespace GFW {

    RenderTarget::RenderTarget(ITextureIn texture, const RenderTargetDesc & desc, IDeviceIn device)
        : ADeviceChild(device)
        , mDesc(desc)
        , mTexture(texture)
    {

    }

    RenderTarget::~RenderTarget()
    {

    }

} // namespace GFW
