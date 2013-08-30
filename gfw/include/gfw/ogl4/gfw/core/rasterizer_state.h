#ifndef __GFW_OGL_RASTERIZER_STATE_H__
#define __GFW_OGL_RASTERIZER_STATE_H__

#include "gfw/base/rasterizer_state.h"

namespace GFW {

    class RasterizerState : public IRasterizerState
    {
    public:
        virtual void
        Apply() const;

    public:
        RasterizerState(const RasterizerStateDesc &, uint32_t descHash);
        ~RasterizerState();

    private:
        RasterizerStateDesc     mDesc;
    };

} // namespace GFW

#endif // __GFW_OGL_RASTERIZER_STATE_H__
