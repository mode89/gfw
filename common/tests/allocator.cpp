#include "gtest\gtest.h"
#include "common\autoref.h"
#include "common\allocator.h"
#include <vector>

namespace AllocatorTests {

    using namespace Common;

    class Allocator: public IAllocator
    {
    public:
        void * Alloc(uint32_t size)
        {
            return new char [size];
        }

        void Free(void * data)
        {
            delete static_cast<char8_t*>(data);
            mFreedData.push_back(data);
        }

        std::vector<void*> mFreedData;
    };

    class Object: public ADeallocatable<AutoRefObject>
    {
    public:
        Object(IAllocator * a)
            : ADeallocatable(a)
        {

        }
    };
    AUTOREF_REFERENCE_DECLARATION(Object);

    ObjectRef CreateObject(IAllocator * a)
    {
        return new(a) Object(a);
    }

    TEST(Allocator, Deallocatable)
    {
        Allocator * allocator = new Allocator;
        ASSERT_TRUE(allocator->mFreedData.size() == 0);

        void * objAddr = NULL;
        {
            ObjectRef obj = CreateObject(allocator);
            objAddr = obj.GetPointer();
            ASSERT_TRUE(objAddr != NULL);
        }
        ASSERT_TRUE(allocator->mFreedData.size() == 1);
        ASSERT_TRUE(allocator->mFreedData[0] == objAddr);

        delete allocator;
    }

} // namespace AllocatorTests
