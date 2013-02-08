#ifndef __GFW_GRAPHICS_OPENGL_CONTEXT_H__
#define __GFW_GRAPHICS_OPENGL_CONTEXT_H__

#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\window.h"
#include "gfw\graphics\opengl\shader.h"
#include "gfw\graphics\opengl\limits.h"
#include "gfw\graphics\opengl\buffer.h"

#include <map>

namespace GFW { namespace OpenGL {

    class Context: public IContext
    {
    public:

        virtual IRenderBufferRef    GetDefaultColorBuffer();

        virtual void                SetShader(ShaderStage, IShaderIn);

        virtual void                SetVertexAttributes(uint32_t number, VertexAttribute []);

        virtual void                SetVertexBuffer(uint32_t slot, IBufferIn);

        virtual void                SetIndexBuffer(IBufferIn);

        virtual void                SetTexture(ShaderStage, uint32_t slot, ITextureIn);

        virtual void                BuildFramebuffer(uint32_t colorBufferCount, IRenderBufferRef color[], IRenderBufferIn depth);

        virtual void                Clear(const ClearParams &);

        virtual void                Draw(const DrawParams &);

        virtual void                Draw(const DrawIndexedParams &);

        virtual void                DrawScreenQuad();

        virtual void                Present();

        virtual void                ClearState();

    public:

        void                        FlushState();

    public:
        Context(Platform::IWindowIn, DeviceIn, Common::IAllocator *);
        ~Context();

    private:
        typedef std::map < uint32_t, uint32_t > tMapProgs;

        DeviceRef                   mDevice;
        OpenglWindowRef             mWindow;

        ShaderRef                   mShaders[SHADER_STAGE_NUMBER];

        VertexAttribute             mVertAttrs[MAX_VERTEX_BUFFER_BIND];

        BufferRef                   mVertexBuffers[MAX_VERTEX_BUFFER_BIND];

        tMapProgs                   mPrograms;
    };

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_CONTEXT_H__
