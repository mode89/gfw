#include "gtest/gtest.h"

#include "common/futex.h"
#include "taskman/taskman.h"

#define TASK_COUNT          1000000
#define TASK_ITER_COUNT     1000

using namespace TaskMan;

struct TaskData
{
    uint64_t *          totalSum;
    Common::Futex *     mutex;
};

void Task(void * data)
{
    uint32_t sum = 0;

    for (int i = 0; i < TASK_ITER_COUNT; ++ i)
    {
        sum += i;
    }

    TaskData * taskData = reinterpret_cast<TaskData*>(data);

    taskData->mutex->Lock();
    *taskData->totalSum += sum;
    taskData->mutex->Unlock();
}

TEST(TaskManagerTests, CreateAndRunTasks)
{
    ITaskManagerRef taskManager = TaskMan::GetInstance();

    Common::AutoPointer<ITaskRef> tasks    = new ITaskRef [TASK_COUNT];
    Common::AutoPointer<TaskData> taskData = new TaskData [TASK_COUNT];

    Common::Futex mutexSum;
    uint64_t sum = 0;

    for (int i = 0; i < TASK_COUNT; ++ i)
    {
        taskData[i].mutex = &mutexSum;
        taskData[i].totalSum = &sum;

        tasks[i] = taskManager->CreateTask(Task);
        tasks[i]->Run(&taskData[i]);
    }

    taskManager->Run();

    ASSERT_TRUE(sum == (1ll * TASK_COUNT * (0 + (TASK_ITER_COUNT - 1)) * TASK_ITER_COUNT / 2));
}
