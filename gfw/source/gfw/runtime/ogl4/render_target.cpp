#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/texture.h"
#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    RenderTarget::RenderTarget( ConstTextureIn texture, const RenderTargetDesc & desc, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mTexture( texture )
    {

    }

    RenderTarget::~RenderTarget()
    {

    }

    ConstITextureRef RenderTarget::GetTexture() const
    {
        return mTexture;
    }

} // namespace GFW
