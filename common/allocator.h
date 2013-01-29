#ifndef __COMMON_ALLOCATOR_H__
#define __COMMON_ALLOCATOR_H__

#include "common\typedefs.h"

namespace Common {

    class IAllocator
    {
    public:
        virtual void *  Alloc(uint32_t size) = 0;

        virtual void    Free(void *) = 0;

        virtual void    FreeAsync(void *) = 0;

        virtual void    GarbageCollect() = 0;

        virtual         ~IAllocator() { }
    };

    template < class BaseClass >
    class ADeallocatable: public BaseClass
    {
    public:
        ADeallocatable(IAllocator * a)
            : mAllocator(a)
        {}

        inline ~ADeallocatable()
        {
            mAllocator->FreeAsync(this);
        }

    protected:
        IAllocator *    mAllocator;
    };

} // namespace Common

void * operator new(uint32_t size, Common::IAllocator *);
void   operator delete(void *, Common::IAllocator *);

#endif // __COMMON_ALLOCATOR_H__
