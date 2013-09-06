#include "gtest/gtest.h"

#include "taskman/taskman.h"

using namespace TaskMan;

void Task(void * data)
{
    (*reinterpret_cast<uint32_t*>(data)) ++;
}

TEST(TaskManagerTests, CreateAndRunTasks)
{
    static const uint32_t TASK_COUNT = 10000000;

    ITaskManagerRef taskManager = TaskMan::GetInstance();

    ITaskRef * tasks = new ITaskRef [TASK_COUNT];

    uint32_t sum = 0;

    for (int i = 0; i < TASK_COUNT; ++ i)
    {
        tasks[i] = taskManager->CreateTask(Task);
        tasks[i]->Run(&sum);
    }

    taskManager->Run();

    ASSERT_TRUE(sum == TASK_COUNT);
}
