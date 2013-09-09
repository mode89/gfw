#include "common/threads.h"

#include "gtest/gtest.h"

namespace CommonTests {

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
        }

    public:
        Runnable()
            : mData(0)
        {}

        uint32_t GetData() { return mData; }

    private:
        uint32_t mData;
    };
    AUTOREF_REFERENCE_DECLARATION(Runnable);

    TEST(Threads, CreateJoin)
    {
        RunnableRef runnable = new Runnable();

        ASSERT_EQ(runnable->GetData(), 0);

        IThreadRef thread = CreateThread(runnable);
        ASSERT_TRUE(thread.IsAttached());

        thread->Join();

        ASSERT_EQ(runnable->GetData(), (0 + ITER_COUNT - 1) * ITER_COUNT / 2);
    }

} // namespace CommonTests
