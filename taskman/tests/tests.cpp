#include "gtest/gtest.h"

#include "common/futex.h"
#include "taskman/taskman.h"

#define TASK_COUNT          100000
#define TASK_ITER_COUNT     1000

using namespace Common;
using namespace TaskMan;

class Task : public ITask
{
public:
    void Run()
    {
        uint32_t sum = 0;

        for (int i = 0; i < mIterCount; ++ i)
        {
            sum += i;
        }

        mMutexSum->Lock();
        *mSum += sum;
        mMutexSum->Unlock();
    }

public:
    Task(uint64_t * sum, Futex * mutexSum, uint32_t iterCnt)
        : mSum(sum)
        , mMutexSum(mutexSum)
        , mIterCount(iterCnt)
    {}

private:
    uint64_t *  mSum;
    Futex *     mMutexSum;
    uint32_t    mIterCount;
};

TEST(TaskManagerTests, CreateAndRunTasks)
{
    ITaskManagerRef taskManager = ITaskManager::GetInstance();

    Common::Futex mutexSum;
    uint64_t sum = 0;

    for (int i = 0; i < TASK_COUNT; ++ i)
    {
        Task * task = new Task(&sum, &mutexSum, TASK_ITER_COUNT);
        taskManager->Enqueue(task);
    }

    taskManager->Run();

    ASSERT_TRUE(sum == (1ll * TASK_COUNT * (0 + (TASK_ITER_COUNT - 1)) * TASK_ITER_COUNT / 2));
}
