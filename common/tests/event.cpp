#include "gtest/gtest.h"

#include "common/event.h"
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

        mEvent->Notify();
    }

public:
    Runnable(Event * event)
        : mData(0)
        , mEvent(event)
    {}

    uint32_t GetData() { return mData; }

private:
    uint32_t mData;
    Event *  mEvent;
};
AUTOREF_REFERENCE_DECLARATION(Runnable);

TEST(Event, CreateWaitNotify)
{
    Event eventDataReady;
    RunnableRef runnable = new Runnable(&eventDataReady);

    ASSERT_EQ(runnable->GetData(), 0);

    IThreadRef thread = CreateThread(runnable);
    eventDataReady.Wait();

    ASSERT_EQ(runnable->GetData(), (0 + ITER_COUNT - 1) * ITER_COUNT / 2);
}
