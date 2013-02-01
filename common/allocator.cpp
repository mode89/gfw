#include "common\allocator.h"
#include "common\autoref.h"

#include <stdlib.h>

namespace Common {

    class DefaultAllocator: public IAllocator
    {
    public:

        virtual void * Alloc(uint32_t size)
        {
            return malloc(size);
        }

        virtual void Free(void * data)
        {
            free(data);
        }

    } gDefaultAllocator;

    IAllocator * Common::GetDefaultAllocator()
    {
        return &gDefaultAllocator;
    }

} // namespace Common
