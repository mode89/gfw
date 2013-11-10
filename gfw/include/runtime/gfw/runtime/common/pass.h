#ifndef __GFW_COMMON_PASS_H__
#define __GFW_COMMON_PASS_H__

#include "gfw/base/pass.h"
#include "gfw/shared/types_fwd.h"

namespace GFW {

    class Pass : public IPass
    {
    public:
        Pass( PassBinaryRef );
        ~Pass();
    };
    AUTOREF_REFERENCE_DECLARATION( Pass );

} // namespace GFW

#endif // __GFW_COMMON_PASS_H__
