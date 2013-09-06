#include "gtest/gtest.h"

#include "common/futex.h"
#include "taskman/taskman.h"

#define TASK_COUNT  1000000
#define ITER_COUNT  10000

using namespace TaskMan;

void Task(void * data)
{
    uint32_t sum = 0;
    for (int i = 0; i < ITER_COUNT; ++ i)
    {
        sum += i;
    }

    static Common::Futex mutexData;
    mutexData.Lock();
    (*reinterpret_cast<uint64_t*>(data)) += sum;
    mutexData.Unlock();
}

TEST(TaskManagerTests, CreateAndRunTasks)
{
    ITaskManagerRef taskManager = TaskMan::GetInstance();

    ITaskRef * tasks = new ITaskRef [TASK_COUNT];

    uint64_t sum = 0;

    for (int i = 0; i < TASK_COUNT; ++ i)
    {
        tasks[i] = taskManager->CreateTask(Task);
        tasks[i]->Run(&sum);
    }

    taskManager->Run();

    ASSERT_TRUE(sum == (1ll * TASK_COUNT * (0 + (ITER_COUNT - 1)) * ITER_COUNT / 2));
}
