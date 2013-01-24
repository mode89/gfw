#ifndef __COMMON_ALLOCATOR_H__
#define __COMMON_ALLOCATOR_H__

#include "common\typedefs.h"

namespace Common {

    class IAllocator
    {
    public:
        virtual void *  Alloc(uint32_t size) = 0;
        virtual void    Free(void *) = 0;
    };

} // namespace Common

void * operator new(uint32_t size, Common::IAllocator *);
void   operator delete(void *, Common::IAllocator *);

#endif // __COMMON_ALLOCATOR_H__
