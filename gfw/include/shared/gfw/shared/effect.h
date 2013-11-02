#ifndef __GFW_SHARED_EFFECT_H__
#define __GFW_SHARED_EFFECT_H__

#include "gfw/shared/technique.h"

#include <vector>

namespace GFW {

    struct EffectDesc
    {
        uint32_t techniqueCount;

        EffectDesc()
            : techniqueCount( 0 )
        {}
    };

    class EffectBinary : public Common::ARefCounted
    {
        typedef std::vector< TechniqueBinaryRef > TechniqueBinaryVec;
    public:
        EffectDesc  mDesc;
    };
    AUTOREF_REFERENCE_DECLARATION(EffectBinary);

} // namespace GFW

#endif // __GFW_SHARED_EFFECT_H__
