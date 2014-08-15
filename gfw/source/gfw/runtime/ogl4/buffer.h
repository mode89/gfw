#ifndef __GFW_RUNTIME_CORE_BUFFER_H__
#define __GFW_RUNTIME_CORE_BUFFER_H__

#include "gfw/buffer.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/ogl4/types_fwd.h"

namespace GFW {

    class Buffer: public ADeviceChild< IBuffer >
    {
    public:
        virtual const BufferDesc &
        GetDesc() const { return mDesc; }

        virtual ResourceType
        GetType() const { return RESOURCE_TYPE_BUFFER; }

        virtual void
        Map( const SubResourceIndex &, MapType, SubResourceData & );

        virtual void
        Unmap( const SubResourceIndex & );

        virtual void
        UpdateSubresource( const SubResourceIndex &, const SubResourceData & );

    public:
        uint32_t
        GetHandle() const       { return mHandle; }

        uint32_t
        GetTarget() const       { return mTarget; }

    public:
        Buffer( const BufferDesc &, const void * initialData, DeviceIn );
        ~Buffer();

    private:
        BufferDesc      mDesc;
        uint32_t        mHandle;
        uint32_t        mTarget;
    };
    SHARED_PTR_TYPEDEFS(Buffer);

} // namespace GFW

#endif // __GFW_RUNTIME_CORE_BUFFER_H__
