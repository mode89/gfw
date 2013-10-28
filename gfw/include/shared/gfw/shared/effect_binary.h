#ifndef __GFW_SHARED_EFFECT_BINARY_H__
#define __GFW_SHARED_EFFECT_BINARY_H__

#include "common/autoref.h"

#include "gfw/shared/technique_binary.h"

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

#endif // __GFW_SHARED_EFFECT_BINARY_H__
