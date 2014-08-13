#ifndef __GFW_SHARED_OGL4_SHADER_H__
#define __GFW_SHARED_OGL4_SHADER_H__

#include <string>
#include <vector>

namespace GFW {

    struct TextureSamplerBinary
    {
        uint32_t    texture;    // texture register index
        uint32_t    sampler;    // sampler register index
        std::string name;       // glsl sampler name

        TextureSamplerBinary()
            : texture( 0 )
            , sampler( 0 )
            , name()
        {}

        template < class Archive >
        void serialize( Archive & ar, unsigned )
        {
            ar & texture;
            ar & sampler;
            ar & name;
        }
    };

    struct ShaderBinaryOgl4
    {
        typedef std::vector< TextureSamplerBinary > TextureSamplerBinaryVec;

        std::string             mSource;
        TextureSamplerBinaryVec mTextureSamplers;

        template < class Archive >
        void serialize( Archive & ar, unsigned )
        {
            ar & mSource;
            ar & mTextureSamplers;
        }
    };

} // namespace GFW

#endif // GFW_SHARED_OGL4_SHADER_H__
