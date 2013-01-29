#ifndef __GFW_GRAPHICS_COMMON_CONTEXT_H__
#define __GFW_GRAPHICS_COMMON_CONTEXT_H__

#include "gfw\graphics\base\context.h"
#include "common\allocator.h"

namespace GFW {

    class AContext: public IContext
    {
    public:
        AContext(Common::IAllocator *);
        ~AContext();

    protected:
        Common::IAllocator *    mAllocator;
    };

} // namespace GFW

#endif // __GFW_GRAPHICS_BASE_CONTEXT_H__
