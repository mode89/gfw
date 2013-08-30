#ifndef __GFW_GRAPHICS_BASE_RENDER_BUFFER_H__
#define __GFW_GRAPHICS_BASE_RENDER_BUFFER_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    struct RenderBufferDesc
    {

    };

    class IRenderBuffer: public Common::ARefCounted
    {
    public:

        virtual void    GetDesc(RenderBufferDesc &) = 0;

        virtual void    GetDesc(TextureDesc &) = 0;

    public:
        virtual ~IRenderBuffer() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IRenderBuffer);

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_RENDER_BUFFER_H__
