#ifndef __GFW_OGL_BUFFER_H__
#define __GFW_OGL_BUFFER_H__

#include "gfw/base/buffer.h"
#include "gfw/base/types_fwd.h"

namespace GFW {

    class Buffer: public IBuffer
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
        Buffer(const BufferDesc &);
        ~Buffer();

    private:
        BufferDesc      mDesc;
        uint32_t        mHandle;
        uint32_t        mTarget;
    };
    AUTOREF_REFERENCE_DECLARATION(Buffer);

} // namespace GFW

#endif // __GFW_OGL_BUFFER_H__
