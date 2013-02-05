#ifndef __COMMON_ALLOCATOR_H__
#define __COMMON_ALLOCATOR_H__

#include "common\typedefs.h"

namespace Common {

    class IAllocator
    {
    public:
        virtual void *  Alloc(uint32_t size) = 0;

        virtual void    Free(void *) = 0;

        virtual         ~IAllocator() { }
    };

    IAllocator * GetDefaultAllocator();

} // namespace Common

inline void * operator new(uint32_t size, Common::IAllocator * a)
{
    return a->Alloc(size);
}

inline void * operator new(uint32_t size, uint32_t number, Common::IAllocator * a)
{
    return a->Alloc(size * number);
}

inline void operator delete(void * data, Common::IAllocator * a)
{
    a->Free(data);
}

#define COMMON_NEW(a, typeName)                 new(a) typeName
#define COMMON_NEW_ARRAY(a, typeName, number)   new(a, number) typeName
#define COMMON_DELETE(a, typeName, pnt)         pnt->~typeName(); operator delete(a, pnt)

#endif // __COMMON_ALLOCATOR_H__
