#include "common\allocator.h"
#include <vector>

namespace Common {

    class DefaultAllocator: public IAllocator
    {
    public:

        virtual void * Alloc(uint32_t size)
        {
            return new char8_t [size];
        }

        virtual void Free(void * data)
        {
            delete static_cast<char*>(data);
        }

        virtual void FreeAsync(void * data)
        {
            mGarbage.push_back(data);
        }

        virtual void GarbageCollect()
        {
            if (mGarbage.size() > 0)
            {
                for (uint32_t i = 0; i < mGarbage.size(); ++ i)
                {
                    delete static_cast<char*>(mGarbage[i]);
                }
                mGarbage.clear();
            }
        }

        ~DefaultAllocator()
        {
            GarbageCollect();
        }

    private:
        std::vector<void*> mGarbage;

    } gDefaultAllocator;

    IAllocator * GetDefaultAllocator()
    {
        return &gDefaultAllocator;
    }

} // namespace Common
