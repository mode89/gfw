#ifndef __GFW_GRAPHICS_OPENGL_SHADER_H__
#define __GFW_GRAPHICS_OPENGL_SHADER_H__

#include "gfw/graphics/base/shader.h"

namespace GFW { namespace OpenGL {

    class Shader: public IShader
    {
    public:
        uint32_t            Compile(const char * source);

    public:
        Shader(ShaderStage, Common::IAllocator *);
        ~Shader();

    private:
        const ShaderStage   mStage;
        uint32_t            mShader;
    };
    AUTOREF_REFERENCE_DECLARATION(Shader);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_SHADER_H__
