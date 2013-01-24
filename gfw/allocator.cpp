#include "gfw\allocator.h"

namespace GFW {

    using namespace Common;

    class Allocator: public IAllocator
    {
    public:

        virtual void * Alloc(uint32_t size)
        {
            return new char8_t [size];
        }

        virtual void Free(void * data)
        {
            delete data;
        }

    } gDefaultAllocator;

    IAllocator * GetDefaultAllocator()
    {
        return &gDefaultAllocator;
    }

} // namespace GFW
