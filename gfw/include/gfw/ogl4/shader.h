#ifndef __GFW_OGL_SHADER_H__
#define __GFW_OGL_SHADER_H__

#include "gfw/base/shader.h"

namespace GFW {

    class Shader: public IShader
    {
    public:
        virtual ShaderStage     GetStage()          { return mStage; }

    public:
        bool                    Compile(const char * source);

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

} // namespace GFW

#endif // __GFW_OGL_SHADER_H__
