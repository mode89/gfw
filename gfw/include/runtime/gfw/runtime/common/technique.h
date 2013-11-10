#ifndef __GFW_COMMON_TECHNIQUE_H__
#define __GFW_COMMON_TECHNIQUE_H__

#include "gfw/base/technique.h"
#include "gfw/shared/types_fwd.h"

namespace GFW {

    class Technique : public ITechnique
    {
    public:
        Technique( TechniqueBinaryRef );
        ~Technique();
    };
    AUTOREF_REFERENCE_DECLARATION( Technique );

} // namespace GFW

#endif // __GFW_COMMON_TECHNIQUE_H__
