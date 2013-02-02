#ifndef __GFW_GRAPHICS_BASE_BUFFER_H__
#define __GFW_GRAPHICS_BASE_BUFFER_H__

#include "common/autoref.h"

namespace GFW {

    class IBuffer: public Common::ARefCounted
    {

    };
    AUTOREF_REFERENCE_DECLARATION(IBuffer);

} // namespace GFW

#endif // _GFW_GRAPHICS_BASE_BUFFER_H__
