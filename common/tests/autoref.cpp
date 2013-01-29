#include "gtest\gtest.h"
#include "common\autoref.h"
#include <vector>

namespace AutoRefTests {

    using namespace Common;

    AUTOREF_FORWARD_DECLARATION(IFactory);

    class IFactory: public AutoRefObject
    {
    public:
	    virtual void DeleteChild(void * child) = 0;
    };

    // An Object that is produced by a Factory

    AUTOREF_FORWARD_DECLARATION(Object);

    class Object : public AutoRefObject
    {
    public:
        uint32_t    GetCounter()		{ return mCounter; }
        bool8_t     IsDestructed()		{ return mDestructed; }

    public:
        Object(IFactory * factory)
		    : mFactory(factory)
		    , mDestructed(false)
        {}

        ~Object()
	    {
		    mFactory->DeleteChild(this);
	    }

    private:
	    IFactory *	mFactory;
        bool8_t     mDestructed;
    };

    class ObjectImpl : public Object
    {
    public:
        uint32_t    GetCounter()        { return mCounter; }
    };
    AUTOREF_REFERENCE_DECLARATION(ObjectImpl);

    // A Factory that produces an Object
    // It knows how to create and delete an Object

    AUTOREF_FORWARD_DECLARATION(Factory);

    class Factory: public IFactory
    {
    public:
	    static FactoryRef CreateInstance()
	    {
		    return new Factory;
	    }

        ObjectRef CreateObject()
        {
            Object * obj = new Object(this);
            return ObjectRef(obj);
        }

        void DeleteChild(void * child)
        {
            mDestroyedChilds.push_back(child);
        }

        void CollectGarbage()
        {
            for (uint32_t i = 0; i < mDestroyedChilds.size(); ++ i)
            {
                delete mDestroyedChilds[i];
            }
            mDestroyedChilds.clear();
        }

        int GetGarbageCapacity()
        {
            return mDestroyedChilds.size();
        }

	    void Release()
	    {
		    CollectGarbage();
		    delete this;
	    }

    private:
        std::vector<void*>  mDestroyedChilds;
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
        ASSERT_TRUE(factory->GetGarbageCapacity() == 0);

        // Create and process an Object
        {
            // Try create an unused Object

            factory->CreateObject();
            ASSERT_TRUE(factory->GetGarbageCapacity() == 1);

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
                ASSERT_TRUE(factory->GetGarbageCapacity() == 2);
                ASSERT_TRUE(ref5->GetCounter() == 6);
            }

            // Check that all references from the previous scope are deleted

            ASSERT_TRUE(ref1->GetCounter() == 1);
        }

        ASSERT_TRUE(factory->GetGarbageCapacity() == 3);
    }

} // namespace AutoRefTests
