#ifndef __GFW_SHARED_OGL4_SHADER_H__
#define __GFW_SHARED_OGL4_SHADER_H__

#include <string>

namespace GFW {

    struct ShaderBinaryOgl4
    {
        std::string mSource;

        template < class Archive >
        void serialize( Archive & ar, unsigned )
        {
            ar & mSource;
        }
    };

} // namespace GFW

#endif // GFW_SHARED_OGL4_SHADER_H__
