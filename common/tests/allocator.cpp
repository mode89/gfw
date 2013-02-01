#include "gtest\gtest.h"
#include "common\autoref.h"
#include "common\allocator.h"
#include <vector>

namespace AllocatorTests {

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
                    delete mGarbage[i];
                }
                mGarbage.clear();
            }
        }

        ~Allocator()
        {
            GarbageCollect();
        }

        std::vector<void*> mGarbage;
    };

    class Object: public ADeallocatable, public AutoRefObject
    {
    public:
        Object(IAllocator * a)
            : ADeallocatable(a)
        {

        }

    public:
        virtual void * GetImplThis() { return this; }
    };
    AUTOREF_REFERENCE_DECLARATION(Object);

    ObjectRef CreateObject(IAllocator * a)
    {
        return new(a) Object(a);
    }

    TEST(Allocator, Deallocatable)
    {
        Allocator * allocator = new Allocator;
        ASSERT_TRUE(allocator->mGarbage.size() == 0);

        // Create an object and check if it was deleted

        void * objAddr = NULL;
        {
            ObjectRef obj = CreateObject(allocator);
            objAddr = obj.GetPointer();
            ASSERT_TRUE(objAddr != NULL);
        }
        ASSERT_TRUE(allocator->mGarbage.size() == 1);
        ASSERT_TRUE(allocator->mGarbage[0] == objAddr);

        delete allocator;
    }

} // namespace AllocatorTests
