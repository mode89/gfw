#ifndef __GFW_SHARED_EFFECT_H__
#define __GFW_SHARED_EFFECT_H__

#include "gfw/shared/types_fwd.h"

#include <list>

namespace GFW {

    class EffectBinary
    {
        typedef std::list< TechniqueBinary > TechniqueBinaryList;
        typedef std::list< ShaderBinary > ShaderBinaryList;

    public:
        ShaderBinaryList    mShaderTable;
        TechniqueBinaryList mTechniques;

        template < class Archive > void
        serialize( Archive & ar, unsigned version )
        {
            ar & mShaderTable;
            ar & mTechniques;
        }
    };

} // namespace GFW

#endif // __GFW_SHARED_EFFECT_H__
