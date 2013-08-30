#ifndef __GFW_GRAPHICS_BASE_TEXTURE_H__
#define __GFW_GRAPHICS_BASE_TEXTURE_H__

#include "gfw/base/types_fwd.h"

namespace GFW {

    struct TextureDesc
    {

    };

    class ITexture: public Common::ARefCounted
    {
    public:
        virtual ~ITexture() {}
    };
    AUTOREF_REFERENCE_DECLARATION(ITexture);

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_TEXTURE_H__
