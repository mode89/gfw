#ifndef __GFW_BASE_TECHNIQUE_H__
#define __GFW_BASE_TECHNIQUE_H__

#include "gfw/base/pass.h"

namespace GFW {

    class ITechnique : public Common::ARefCounted
    {
    public:
        virtual
        ~ITechnique() {}
    };
    AUTOREF_REFERENCE_DECLARATION( ITechnique );

} // namespace GFW

#endif // __GFW_BASE_TECHNIQUE_H__
