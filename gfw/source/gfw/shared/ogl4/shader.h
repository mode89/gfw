#ifndef __GFW_SHARED_OGL4_SHADER_H__
#define __GFW_SHARED_OGL4_SHADER_H__

#include <string>
#include <vector>

namespace GFW {

    struct ShaderBinaryOgl4
    {
        typedef std::pair< std::string, std::string > TextureSamplerPair;
        typedef std::vector< TextureSamplerPair > TextureSamplerPairVec;

        std::string             mSource;
        TextureSamplerPairVec   mTextureSamplerPairs;

        template < class Archive >
        void serialize( Archive & ar, unsigned )
        {
            ar & mSource;
            ar & mTextureSamplerPairs;
        }
    };

} // namespace GFW

#endif // GFW_SHARED_OGL4_SHADER_H__
