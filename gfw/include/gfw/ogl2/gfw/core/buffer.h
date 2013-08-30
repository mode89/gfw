#ifndef __GFW_OGL_BUFFER_H__
#define __GFW_OGL_BUFFER_H__

#include "gfw/base/buffer.h"

namespace GFW {

    class Buffer: public IBuffer
    {
    public:
        uint32_t        Init(const void * initialData);

        inline
        uint32_t        GetBufferObject()   { return mBuffer; }

    public:
        Buffer(const BufferDesc &);
        ~Buffer();

    private:
        BufferDesc      mDesc;
        uint32_t        mBuffer;
    };
    AUTOREF_REFERENCE_DECLARATION(Buffer);

} // namespace GFW

#endif // __GFW_OGL_BUFFER_H__
