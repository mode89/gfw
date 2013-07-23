#ifndef __GFW_GRAPHICS_OPENGL_SHADER_H__
#define __GFW_GRAPHICS_OPENGL_SHADER_H__

#include "gfw/graphics/base/shader.h"

namespace GFW { namespace OpenGL {

    class Shader: public IShader
    {
    public:
        virtual ShaderStage     GetStage()          { return mStage; }

    public:
        uint32_t                Compile(const char * source);

        inline
        uint32_t                GetHash()           { return mHash; }

        inline
        uint32_t                GetShaderObject()   { return mShader; }

    public:
        Shader(ShaderStage);
        ~Shader();

    private:
        const ShaderStage       mStage;
        uint32_t                mShader;
        uint32_t                mHash;
    };
    AUTOREF_REFERENCE_DECLARATION(Shader);

}} // namespace GFW::OpenGL

#endif // __GFW_GRAPHICS_OPENGL_SHADER_H__
