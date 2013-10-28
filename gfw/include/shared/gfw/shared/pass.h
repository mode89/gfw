#ifndef __GFW_SHARED_PASS_H__
#define __GFW_SHARED_PASS_H__

#include "common/autoref.h"

namespace GFW {

    struct PassDesc
    {
    };

    class PassBinary : public Common::ARefCounted
    {
    public:
    };
    AUTOREF_REFERENCE_DECLARATION( PassBinary );

} // namespace GFW

#endif // __GFW_SHARED_PASS_H__
