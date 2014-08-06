#include "gfw/runtime/ogl4/device.h"
#include "gfw/runtime/ogl4/render_target.h"
#include "gfw/runtime/ogl4/texture.h"
#include "gfw/runtime/common/device_child.inl"

namespace GFW {

    RenderTarget::RenderTarget( ConstTextureIn texture, const RenderTargetDesc & desc, DeviceIn device )
        : ADeviceChild( device )
        , mDesc( desc )
        , mTexture( texture )
        , mIsSwapChain( false )
        , mTextureDesc( &texture->GetDesc() )
    {

    }

    RenderTarget::RenderTarget( const TextureDesc & textureDesc, const RenderTargetDesc & desc )
        : ADeviceChild( nullptr )
        , mDesc( desc )
        , mTexture()
        , mIsSwapChain( true )
        , mTextureDesc( new TextureDesc( textureDesc ) )
    {

    }

    RenderTarget::~RenderTarget()
    {
        if ( mIsSwapChain )
        {
            delete mTextureDesc;
        }
    }

    ConstITextureRef RenderTarget::GetTexture() const
    {
        return mTexture;
    }

} // namespace GFW
