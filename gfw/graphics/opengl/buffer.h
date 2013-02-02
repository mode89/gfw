#ifndef __GFW_GRAPHICS_OPENGL_BUFFER_H__
#define __GFW_GRAPHICS_OPENGL_BUFFER_H__

#include "gfw/graphics/base/buffer.h"

namespace GFW { namespace OpenGL {

    class Buffer: public IBuffer
    {
    public:
        uint32_t        Init(const void * initialData);

        inline
        uint32_t        GetBufferObject()   { return mBuffer; }

    public:
        Buffer(const BufferDesc &, Common::IAllocator *);
        ~Buffer();

    private:
        BufferDesc      mDesc;
        uint32_t        mBuffer;
    };
    AUTOREF_REFERENCE_DECLARATION(Buffer);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_BUFFER_H__
