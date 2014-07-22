#ifndef __GFW_SHARED_TECHNIQUE_H__
#define __GFW_SHARED_TECHNIQUE_H__

#include "gfw/shared/types_fwd.h"

#include <string>
#include <list>

namespace GFW {

    class TechniqueBinary
    {
        typedef std::list< PassBinary > PassBinaryList;

    public:
        std::string     mName;
        PassBinaryList  mPasses;

        template < class Archive > void
        serialize( Archive & ar, unsigned version )
        {
            ar & mName;
            ar & mPasses;
        }
    };

} // namespace GFW

#endif // __GFW_SHARED_TECHNIQUE_H__
