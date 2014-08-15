#ifndef __GFW_RUNTIME_CORE_TEXTURE_H__
#define __GFW_RUNTIME_CORE_TEXTURE_H__

#include "gfw/texture.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/ogl4/types_fwd.h"

namespace GFW {

    class Texture : public ADeviceChild<ITexture>
    {
    public:
        virtual const TextureDesc &
        GetDesc() const { return mDesc; }

        virtual ResourceType
        GetType() const { return RESOURCE_TYPE_TEXTURE; }

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
        Texture( const TextureDesc &, const SubResourceData * initialData, DeviceIn );
        ~Texture();

    private:
        TextureDesc     mDesc;
        uint32_t        mHandle;
    };
    SHARED_PTR_TYPEDEFS(Texture);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_TEXTURE_H__
