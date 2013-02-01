#include "gtest\gtest.h"
#include "common\autoref.h"
#include <vector>

namespace AutoRefTests {

    using namespace Common;

    AUTOREF_FORWARD_DECLARATION(Factory);
    AUTOREF_FORWARD_DECLARATION(Object);
    AUTOREF_FORWARD_DECLARATION(ObjectImpl);

    // An Object that is produced by a Factory

    class Object : public ARefCounted
    {
    public:
        uint32_t GetCounter() { return mRefCounter; }

    public:
        Object(FactoryIn factory, IAllocator * a)
		    : mFactory(factory)
        {
            mAllocator = a;
        }

    private:
	    FactoryRef mFactory;
    };

    class ObjectImpl : public Object
    {
    public:
        uint32_t GetCounter() { return mRefCounter; }
    };

    // A Factory that produces an Object
    // It knows how to create and delete an Object

    class Factory: public ARefCounted
    {
    public:
	    static FactoryRef CreateInstance()
	    {
            IAllocator * a = GetDefaultAllocator();
            return new(a) Factory(a);
        }

        Factory(IAllocator * a)
        {
            mAllocator = a;
        }

        ObjectRef CreateObject()
        {
            return new(mAllocator) Object(this, mAllocator);
        }

        uint32_t GetCounter() { return mRefCounter; }
    };

    // Try to receive and return a reference

    ObjectRef ReturnReference(ObjectIn ref)
    {
        ObjectImplRef impl = ref.StaticCast<ObjectImpl>();
        uint32_t cnt = ref->GetCounter();
        return impl.StaticCast<Object>();
    }

    TEST(AutoRef, Test)
    {
	    FactoryRef factory = Factory::CreateInstance();

        // Create and process an Object
        {
            // Try create an unused Object

            factory->CreateObject();

            // Create an Object for processing

            ObjectRef ref1 = factory->CreateObject();
            ASSERT_TRUE(ref1->GetCounter() == 1);

            // Process the Object
            {
                // Try to create a reference from reference

                ObjectRef ref2(ref1);
                ASSERT_TRUE(ref1->GetCounter() == 2);

                ObjectRef ref3(0);

                // Try to copy a reference

                ref3 = ref1;
                ASSERT_TRUE(ref2->GetCounter() == 3);

                ObjectRef ref4;

                // Try to pass the reference to a function
                // and receive a reference

                ref4 = ReturnReference(ref2);
                ASSERT_TRUE(ref3->GetCounter() == 4);

                // Try self-assignment

                ref2 = ref2;
                ASSERT_TRUE(ref4->GetCounter() == 4);

                ObjectRef ref5;

                // Try to receive reference as a result of an assignment

                ref5 = ref3 = ref3;
                ASSERT_TRUE(ref1->GetCounter() == 5);

                // Try to cast reference

                ObjectImplRef impl = ref5.StaticCast<ObjectImpl>();
                ASSERT_TRUE(impl->GetCounter() == 6);

                // Construct null-reference and assign a value to it

                ObjectRef ref6;
                ref5 = ref6;
                ASSERT_TRUE(ref4->GetCounter() == 5);

                // Try to loose reference

                ref5 = factory->CreateObject();
                ASSERT_TRUE(ref5->GetCounter() == 1);

                ref5 = ref4;
                ASSERT_TRUE(ref5->GetCounter() == 6);
            }

            // Check that all references from the previous scope are deleted

            ASSERT_TRUE(ref1->GetCounter() == 1);
        }

        ASSERT_TRUE(factory->GetCounter() == 1);
    }

} // namespace AutoRefTests
