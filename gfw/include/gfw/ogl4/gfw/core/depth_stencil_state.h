#ifndef __GFW_OGL_DEPTH_STENCIL_STATE_H__
#define __GFW_OGL_DEPTH_STENCIL_STATE_H__

#include "gfw/base/depth_stencil_state.h"

namespace GFW {

    class DepthStencilState : public IDepthStencilState
    {
    public:
        virtual const DepthStencilStateDesc &
        GetDesc()   { return mDesc; }

        virtual void
        Apply() const;

    public:
        DepthStencilState(const DepthStencilStateDesc &, uint32_t descHash);
        ~DepthStencilState();

    private:
        DepthStencilStateDesc   mDesc;
    };

} // napespace GFW

#endif __GFW_OGL_DEPTH_STENCIL_STATE_H__
