#ifndef __GFW_ALLOCATOR_H__
#define __GFW_ALLOCATOR_H__

#include "common\allocator.h"

#define GFW_NEW(a, className)               new((a)) className
#define GFW_DELETE(a, pointerToObject)      operator delete((pointerToObject), (a))
#define GFW_NEW_ARRAY(a, className, number) new((number), (a)) className

namespace GFW {

    Common::IAllocator * GetDefaultAllocator();

} // namespace GFW

#endif // __GFW_ALLOCATOR_H__

