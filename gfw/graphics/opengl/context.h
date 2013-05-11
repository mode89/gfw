#ifndef __GFW_GRAPHICS_OPENGL_CONTEXT_H__
#define __GFW_GRAPHICS_OPENGL_CONTEXT_H__

#include "gfw\graphics\opengl\device.h"
#include "gfw\graphics\opengl\shader.h"
#include "gfw\graphics\opengl\limits.h"
#include "gfw\graphics\opengl\buffer.h"

#include <map>

namespace GFW { namespace OpenGL {

    class Context: public IContext
    {
    public:

        virtual void                SetShader(ShaderStage, IShaderIn);

        virtual void                SetVertexAttributes(uint32_t number, VertexAttribute []);

        virtual void                SetVertexBuffer(uint32_t slot, IBufferIn);

        virtual void                SetIndexBuffer(IBufferIn);

        virtual void                SetTexture(ShaderStage, uint32_t slot, ITextureIn);

        virtual void                SetFrameBuffer(uint32_t colorBufferCount, IRenderBufferRef color[], IRenderBufferIn depth);

        virtual void                BeginScene();

        virtual void                EndScene();

        virtual void                Clear(const ClearParams &);

        virtual void                Draw(const DrawParams &);

        virtual void                Draw(const DrawIndexedParams &);

        virtual void                DrawScreenQuad();

        virtual void                ClearState();

    public:

        void                        FlushState();

    public:
        Context(DeviceIn);
        ~Context();

    private:
        typedef std::map < uint32_t, uint32_t > tMapProgs;

        DeviceRef                   mDevice;

        NativeContext               mNativeContext;

        ShaderRef                   mShaders[SHADER_STAGE_COUNT];

        VertexAttribute             mVertAttrs[MAX_VERTEX_BUFFER_BIND];

        BufferRef                   mVertexBuffers[MAX_VERTEX_BUFFER_BIND];

        tMapProgs                   mPrograms;
    };

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_CONTEXT_H__
