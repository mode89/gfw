#ifndef __GFW_TEXTURE_H__
#define __GFW_TEXTURE_H__

#include "gfw/format.h"
#include "gfw/resource.h"
#include "gfw/types_fwd.h"

namespace GFW {

    struct TextureDesc : public ResourceDesc
    {
        uint32_t    width;
        uint32_t    height;
        uint32_t    mipLevels;
        Format      format;

        TextureDesc()
            : width( 0 )
            , height( 0 )
            , mipLevels( 0 )
            , format( FORMAT_UNKNOWN )
        {}
    };

    class ITexture: public IResource
    {
    public:
        virtual const TextureDesc &
        GetDesc() const = 0;

        virtual
        ~ITexture() {}
    };
    SHARED_PTR_TYPEDEFS( ITexture );

} // namespace GFW

#endif // __GFW_TEXTURE_H__
