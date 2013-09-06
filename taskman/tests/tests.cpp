#include "gtest/gtest.h"

#include "taskman/taskman.h"

using namespace TaskMan;

void Task(void * data)
{
    (*reinterpret_cast<uint32_t*>(data)) ++;
}

TEST(TaskManagerTests, Tasks)
{
    ITaskManagerRef taskManager = TaskMan::GetInstance();

    static const uint32_t taskCnt = 102400;
    ITaskRef tasks[taskCnt];

    uint32_t sum = 0;

    for (int i = 0; i < 1024; ++ i)
    {
        tasks[i] = taskManager->CreateTask(Task);
        tasks[i]->Run(&sum);
    }

    taskManager->Run();

    ASSERT_TRUE(sum == taskCnt);
}
