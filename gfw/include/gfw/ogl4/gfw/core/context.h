#ifndef __GFW_OGL_CONTEXT_H__
#define __GFW_OGL_CONTEXT_H__

#include "gfw/base/context.h"
#include "gfw/base/shader.h"
#include "gfw/base/vertex_attribute.h"

#include "gfw/core/limits.h"
#include "gfw/core/types_fwd.h"

#include <map>

namespace GFW {

    class Context: public IContext
    {
    public:
        virtual void
        SetShader(int32_t stage, IShaderIn);

        virtual void
        SetVertexAttributes(uint32_t number, VertexAttribute []);

        virtual void
        SetVertexBuffer(uint32_t slot, IBufferIn);

        virtual void
        SetIndexBuffer(IBufferIn);

        virtual void
        SetTexture(int32_t stage, uint32_t slot, ITextureIn);

        virtual void
        SetFrameBuffer(uint32_t colorBufferCount, IRenderBufferRef color[], IRenderBufferIn depth);

        virtual void
        BeginScene();

        virtual void
        EndScene();

        virtual void
        Clear(const ClearParams &);

        virtual void
        Draw(const DrawParams &);

        virtual void
        Draw(const DrawIndexedParams &);

        virtual void
        DrawScreenQuad();

        virtual void
        ClearState();

    public:
        void
        FlushState();

    public:
        Context(DeviceIn, IDrawingContextIn);
        ~Context();

    private:
        typedef std::map < uint32_t, uint32_t > tMapProgs;

        DeviceRef                   mDevice;
        IDrawingContextRef          mDrawingContext;

        RenderingContext            mContextGL;

        ShaderRef                   mShaders[SHADER_STAGE_COUNT];

        VertexAttribute             mVertAttrs[MAX_VERTEX_BUFFER_BIND];

        BufferRef                   mVertexBuffers[MAX_VERTEX_BUFFER_BIND];

        BufferRef                   mIndexBuffer;

        tMapProgs                   mPrograms;
    };

} // namespace GFW

#endif // __GFW_OGL_CONTEXT_H__
