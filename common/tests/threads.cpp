#include "common/threads.h"

#include "gtest/gtest.h"

namespace CommonTests {

    using namespace Common;

    uint32_t g_Data = 0;

    uint32_t Foo(void * data)
    {
        g_Data = *static_cast<uint32_t*>(data);
        return 0;
    }

    TEST(Threads, CreateJoin)
    {
        uint32_t data = rand();

        ASSERT_TRUE(g_Data == 0);

        IThreadRef thread = CreateThread(Foo, &data);
        ASSERT_TRUE(thread.IsAttached());

        thread->Join();

        ASSERT_TRUE(g_Data == data);
    }

} // namespace CommonTests
