#include "gfw/runtime/core/device.h"
#include "gfw/runtime/core/render_target.h"
#include "gfw/runtime/core/texture.h"
#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    RenderTarget::RenderTarget( ConstITextureIn texture, const RenderTargetDesc & desc, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mTexture( std::static_pointer_cast< const Texture >( texture ) )
    {

    }

    RenderTarget::~RenderTarget()
    {

    }

} // namespace GFW
