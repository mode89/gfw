#ifndef __GFW_CORE_BUFFER_H__
#define __GFW_CORE_BUFFER_H__

#include "gfw/base/buffer.h"
#include "gfw/runtime/common/device_child.h"
#include "gfw/runtime/core/types_fwd.h"

namespace GFW {

    class Buffer: public ADeviceChild<IBuffer>
    {
    public:
        virtual const BufferDesc &
        GetDesc()       { return mDesc; }

        virtual void *
        Map(uint32_t mapFlags);

        virtual void
        Unmap();

        virtual void
        UpdateSubresource(const void * data, uint32_t subResourceIndex = 0);

    public:
        bool
        Init(const void * initialData);

        inline uint32_t
        GetHandle()     { return mHandle; }

        inline uint32_t
        GetTarget()     { return mTarget; }

    public:
        Buffer(const BufferDesc &, IDeviceRef);
        ~Buffer();

    private:
        BufferDesc      mDesc;
        uint32_t        mHandle;
        uint32_t        mTarget;
    };
    AUTOREF_REFERENCE_DECLARATION(Buffer);

} // namespace GFW

#endif // __GFW_CORE_BUFFER_H__
