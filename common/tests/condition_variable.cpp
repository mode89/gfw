#include "gtest/gtest.h"

#include "common/condition_variable.h"
#include "common/threads.h"

using namespace Common;

#define ITER_COUNT  10000

class Runnable : public IRunnable
{
public:
    virtual void Run()
    {
        for (int i = 0; i < ITER_COUNT; ++ i)
        {
            mData += i;
        }

        mCondVar->Notify();
    }

public:
    Runnable(ConditionVariable * condVar)
        : mData(0)
        , mCondVar(condVar)
    {}

    uint32_t GetData() { return mData; }

private:
    uint32_t            mData;
    ConditionVariable * mCondVar;
};
AUTOREF_REFERENCE_DECLARATION(Runnable);

TEST(ConditionVariable, CreateWaitNotify)
{
    Mutex mutex;
    ConditionVariable condVar;
    RunnableRef runnable = new Runnable(&condVar);

    ASSERT_EQ(runnable->GetData(), 0);

    IThreadRef thread = CreateThread(runnable);
    mutex.Lock();
    condVar.Wait(mutex);
    mutex.Unlock();

    ASSERT_EQ(runnable->GetData(), (0 + ITER_COUNT - 1) * ITER_COUNT / 2);
}
