#ifndef __GFW_BASE_CONTEXT_H__
#define __GFW_BASE_CONTEXT_H__

#include "common/autoref.h"
#include "gfw/base/clear_params.h"
#include "gfw/base/vertex_attribute.h"
#include "gfw/base/draw_params.h"
#include "gfw/base/shader.h"
#include "gfw/base/buffer.h"

namespace GFW {

    class IContext: public Common::ARefCounted
    {
    public:

        virtual void    SetShader(ShaderStage, IShaderIn) = 0;

        virtual void    SetVertexAttributes(uint32_t number, VertexAttribute []) = 0;

        virtual void    SetVertexBuffer(uint32_t slot, IBufferIn) = 0;

        virtual void    Clear(const ClearParams &) = 0;

        virtual void    Draw(const DrawParams &) = 0;

        virtual void    ClearState() = 0;

    public:
        virtual         ~IContext() { }
    };
    AUTOREF_REFERENCE_DECLARATION(IContext);

} // namespace GFW

#endif // __GFW_BASE_CONTEXT_H__
