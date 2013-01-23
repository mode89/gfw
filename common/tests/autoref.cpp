#include "common\autoref.h"
#include "gtest\gtest.h"
#include <vector>

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

	void Release()
	{
		mFactory->DeleteChild(this);
	}

private:
	IFactory *	mFactory;
    bool8_t     mDestructed;
};

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
    uint32_t cnt = ref->GetCounter();
    return ref;
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

            ObjectRef ref4(0);

            // Try to pass the reference to a function
            // and receive a reference

            ref4 = ReturnReference(ref2);
            ASSERT_TRUE(ref3->GetCounter() == 4);

            // Try self-assignment

            ref2 = ref2;
            ASSERT_TRUE(ref4->GetCounter() == 4);

            ObjectRef ref5(0);

            // Try to receive reference as a result of an assignment

            ref5 = ref3 = ref3;
            ASSERT_TRUE(ref1->GetCounter() == 5);
        }

        // Check that all references from the previous scope are deleted

        ASSERT_TRUE(ref1->GetCounter() == 1);
    }
}
