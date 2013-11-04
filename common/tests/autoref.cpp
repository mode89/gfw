#include "gtest/gtest.h"
#include "common/autoref.h"

using namespace Common;

class IObject : public ARefCounted
{
public:
    virtual int32_t
    GetData() = 0;

    virtual void
    SetData(int32_t) = 0;

    virtual
    ~IObject() {}
};
AUTOREF_REFERENCE_DECLARATION(IObject);

class IFactory : public ARefCounted
{
public:
    virtual IObjectRef
    CreateObject() = 0;

    ~IFactory() {}
};
AUTOREF_REFERENCE_DECLARATION(IFactory);

class Object : public IObject
{
public:
    virtual int32_t
    GetData() { return mData; }

    virtual void
    SetData(int32_t data) { mData = data; }

public:
    Object(IFactoryIn factory)
		: mFactory(factory)
        , mData(0)
    {}

    ~Object()
    {

    }

private:
    IFactoryRef mFactory;
    int32_t     mData;
};
AUTOREF_REFERENCE_DECLARATION(Object);

class Factory: public IFactory
{
public:
    virtual IObjectRef CreateObject()
    {
        return new Object(this);
    }
};
AUTOREF_REFERENCE_DECLARATION(Factory);

class AutoRefTests : public ::testing::Test
{
public:
    void SetUp()
    {
        mFactory = CreateFactory();
    }

    void TearDown()
    {
        mFactory.Detach();
    }

protected:
    IFactoryRef CreateFactory()
    {
        return new Factory;
    }

protected:
    IFactoryRef     mFactory;
};

// After setting up the test there is only one reference to the factory object
TEST_F(AutoRefTests, FactoryReference)
{
    ASSERT_TRUE(mFactory->GetRefCounter() == 1);
}

// Interface class of the object must have a virtual ctor to successfully destruct the instance of the object
TEST_F(AutoRefTests, IfaceVirtualCtor)
{
    ASSERT_TRUE(mFactory->GetRefCounter() == 1);
    {
        IObjectRef object = mFactory->CreateObject();
        ASSERT_TRUE(mFactory->GetRefCounter() == 2);
        ASSERT_TRUE(object->GetRefCounter() == 1);
    }
    ASSERT_TRUE(mFactory->GetRefCounter() == 1);
}

// Test IsAttached() method
TEST_F(AutoRefTests, IsAttached)
{
    IObjectRef object;
    ASSERT_TRUE(!object.IsAttached());

    object = mFactory->CreateObject();
    ASSERT_TRUE(object.IsAttached());
}

// Test IsNull() method
TEST_F(AutoRefTests, IsNull)
{
    IObjectRef object;
    ASSERT_TRUE(object.IsNull());

    object = mFactory->CreateObject();
    ASSERT_TRUE(!object.IsNull());
}

// Construct from a null pointer
TEST_F(AutoRefTests, ConstructFromNullPointer)
{
    IObjectRef ref(NULL);
    ASSERT_TRUE(ref.IsNull());
}

// Construct from a pointer
TEST_F(AutoRefTests, ConstructFromPointer)
{
    IObject * pnt = new Object(NULL);
    IObjectRef ref = pnt;
    ASSERT_TRUE(ref.GetPointer() == pnt);
    ASSERT_TRUE(ref->GetRefCounter() == 1);
}

// Construct from a reference
TEST_F(AutoRefTests, ConstructFromReference)
{
    IObjectRef ref1 = mFactory->CreateObject();
    ASSERT_TRUE(ref1->GetRefCounter() == 1);

    IObjectRef ref2(ref1);
    ASSERT_TRUE(ref1->GetRefCounter() == 2);
    ASSERT_TRUE(ref2->GetRefCounter() == 2);
    ASSERT_TRUE(ref1.GetPointer() == ref2.GetPointer());
}

// Assignment operator
TEST_F(AutoRefTests, AssignmentOperator)
{
    IObjectRef ref1 = mFactory->CreateObject();
    IObjectRef ref2 = mFactory->CreateObject();
    ASSERT_TRUE(ref1->GetRefCounter() == 1);
    ASSERT_TRUE(ref2->GetRefCounter() == 1);

    // Backup ref2
    IObjectRef ref3 = ref2;
    ASSERT_TRUE(ref2->GetRefCounter() == 2);
    ASSERT_TRUE(ref2.GetPointer() == ref3.GetPointer());

    // Assignment
    ref2 = ref1;
    ASSERT_TRUE(ref1->GetRefCounter() == 2);
    ASSERT_TRUE(ref2->GetRefCounter() == 2);
    ASSERT_TRUE(ref1.GetPointer() == ref2.GetPointer());
    ASSERT_TRUE(ref3->GetRefCounter() == 1);
    ASSERT_TRUE(ref3.GetPointer() != ref2.GetPointer());
}

// Self-assignment
TEST_F(AutoRefTests, SelfAssignment)
{
    IObjectRef ref = mFactory->CreateObject();
    ASSERT_TRUE(ref->GetRefCounter() == 1);
    ref = ref;
    ASSERT_TRUE(ref->GetRefCounter() == 1);
}

// Assignment result
TEST_F(AutoRefTests, AssignmentResult)
{
    IObjectRef ref1 = mFactory->CreateObject();
    IObjectRef ref2, ref3;
    ASSERT_TRUE(ref1->GetRefCounter() == 1);

    ref3 = ref2 = ref1;
    ASSERT_TRUE(ref1->GetRefCounter() == 3);
    ASSERT_TRUE(ref2->GetRefCounter() == 3);
    ASSERT_TRUE(ref3->GetRefCounter() == 3);
    ASSERT_TRUE(ref2.GetPointer() == ref1.GetPointer());
    ASSERT_TRUE(ref3.GetPointer() == ref1.GetPointer());
}

uint32_t AutoRefTests_In(IObjectIn ref)
{
    return ref->GetRefCounter();
}

// Test *In reference
TEST_F(AutoRefTests, In)
{
    IObjectRef ref = mFactory->CreateObject();
    ASSERT_TRUE(ref->GetRefCounter() == 1);
    ASSERT_TRUE(AutoRefTests_In(ref) == 1);
    ASSERT_TRUE(ref->GetRefCounter() == 1);
}

// Static casting
TEST_F(AutoRefTests, StaticCast)
{
    IObjectRef ref1 = mFactory->CreateObject();
    ASSERT_TRUE(ref1->GetRefCounter() == 1);

    const int32_t data = 0x12345321;
    ref1->SetData(data);

    ObjectRef ref2 = ref1.StaticCast<Object>();
    ASSERT_TRUE(ref1->GetRefCounter() == 2);
    ASSERT_TRUE(ref2->GetRefCounter() == 2);
    ASSERT_TRUE(ref1->GetData() == data);
    ASSERT_TRUE(ref2->GetData() == data);
}

TEST_F(AutoRefTests, CastCtor)
{
    ObjectRef ref = mFactory->CreateObject();
    ASSERT_TRUE(ref->GetRefCounter() == 1);
    ASSERT_TRUE(AutoRefTests_In(ref) == 2);
    ASSERT_TRUE(ref->GetRefCounter() == 1);
}

TEST_F(AutoRefTests, AutoPointerBracket)
{
    AutoPointer<uint32_t> autoPnt = new uint32_t [100];

    for (int i = 0; i < 100; ++ i)
    {
        autoPnt[i] = i * 2;
        ASSERT_TRUE( autoPnt[i] == ( i * 2 ) );
    }
}

TEST_F( AutoRefTests, AutoPointerGetPointer )
{
    uint32_t * data = new uint32_t [100];
    AutoPointer<uint32_t> ptr = data;
    ASSERT_TRUE( ptr.GetPointer() == data );
}

TEST_F(AutoRefTests, ArrayOfRefs)
{
    IObjectRef * objRefArray = new IObjectRef [100];
    delete [] objRefArray;

    AutoPointer<IObjectRef> objRefAutoArray = new IObjectRef [1000];
}

TEST_F(AutoRefTests, CompareRefs)
{
    IObjectRef ref1 = mFactory->CreateObject();
    IObjectRef ref2 = mFactory->CreateObject();
    ObjectRef  ref3 = mFactory->CreateObject();

    ASSERT_TRUE(ref1 != ref2);
    ASSERT_TRUE(ref1 != ref3);

    ref1 = ref2;
    ASSERT_TRUE(ref1 == ref2);

    ref1 = ref3;
    ASSERT_TRUE(ref1 == ref3);
}
