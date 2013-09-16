#ifndef __GFW_CORE_TEXTURE_H__
#define __GFW_CORE_TEXTURE_H__

#include "gfw/base/texture.h"
#include "gfw/common/device_child.h"
#include "gfw/core/types_fwd.h"

namespace GFW {

    class Texture : public ADeviceChild<ITexture>
    {
    public:
        virtual void *
        Map(uint32_t mapFlags);

        virtual void
        Unmap();

        virtual void
        UpdateSubresource(const void * data, uint32_t subResourceIndex /* = 0 */);

    public:
        inline uint32_t
        GetHandle() const { return mHandle; }

    public:
        Texture(const TextureDesc &, const void * initialData, IDeviceIn);
        ~Texture();

    private:
        TextureDesc     mDesc;
        uint32_t        mHandle;
    };
    AUTOREF_REFERENCE_DECLARATION(Texture);

} // namespace GFW

#endif // __GFW_CORE_TEXTURE_H__
