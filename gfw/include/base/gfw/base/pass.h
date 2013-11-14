#ifndef __GFW_BASE_PASS_H__
#define __GFW_BASE_PASS_H__

#include "common/autoref.h"

namespace GFW {

    class IPass : public Common::ARefCounted
    {
    public:
        virtual void
        Dispatch() = 0;

        virtual
        ~IPass() {}
    };
    AUTOREF_REFERENCE_DECLARATION( IPass );

} // namespace GFW

#endif // __GFW_BASE_PASS_H__
