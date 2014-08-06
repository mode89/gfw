#ifndef __GFW_RUNTIME_CORE_RENDER_TARGET_H__
#define __GFW_RUNTIME_CORE_RENDER_TARGET_H__

#include "gfw/render_target.h"
#include "gfw/types_fwd.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/ogl4/types_fwd.h"

namespace GFW {

    class RenderTarget : public ADeviceChild< IRenderTarget >
    {
    public:
        virtual const RenderTargetDesc &
        GetDesc() const { return mDesc; }

        virtual const TextureDesc &
        GetTextureDesc() const { return *mTextureDesc; }

        virtual ConstITextureRef
        GetTexture() const;

    public:
        bool
        IsSwapChain() const { return mIsSwapChain; }

    public:
        RenderTarget( ConstTextureIn, const RenderTargetDesc &, DeviceIn );
        RenderTarget( const TextureDesc &, const RenderTargetDesc & ); // Swap-chain's buffer
        ~RenderTarget();

    private:
        RenderTargetDesc    mDesc;
        ConstTextureRef     mTexture;

        bool                mIsSwapChain;
        const TextureDesc * mTextureDesc;

    private:
        RenderTarget( const RenderTarget & );
        RenderTarget & operator= ( const RenderTarget & );
    };
    SHARED_PTR_TYPEDEFS(RenderTarget);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_RENDER_TARGET_H__
