#include "common\allocator.h"

void * operator new(uint32_t size, Common::IAllocator * a)
{
    return a->Alloc(size);
}

void operator delete(void * data, Common::IAllocator * a)
{
    a->Free(data);
}
