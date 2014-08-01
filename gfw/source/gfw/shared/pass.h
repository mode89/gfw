#ifndef __GFW_SHARED_PASS_H__
#define __GFW_SHARED_PASS_H__

#include "gfw/shader_stage.h"
#include "gfw/shared/types_fwd.h"

#include <string>

namespace GFW {

    class PassBinary
    {
    public:
        std::string             mName;
        const ShaderBinary *    mShaders[ SHADER_STAGE_COUNT ];

        template < class Archive > void
        serialize( Archive & ar, unsigned version )
        {
            ar & mName;
            ar & mShaders;
        }
    };

} // namespace GFW

#endif // __GFW_SHARED_PASS_H__
