#include "gtest/gtest.h"

#include "common/mutex.h"
#include "taskman/taskman.h"

#define TASK_COUNT          1000000
#define TASK_ITER_COUNT     1000

using namespace Common;
using namespace TaskMan;

class Task : public IRunnable
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
    Task(uint64_t * sum, Mutex * mutexSum, uint32_t iterCnt)
        : mSum(sum)
        , mMutexSum(mutexSum)
        , mIterCount(iterCnt)
    {}

private:
    uint64_t *  mSum;
    Mutex *     mMutexSum;
    uint32_t    mIterCount;
};

TEST(TaskManagerTests, CreateAndRunTasks)
{
    ITaskManagerRef taskManager = ITaskManager::GetInstance();

    Common::Mutex mutexSum;
    uint64_t sum = 0;

    for (int i = 0; i < TASK_COUNT; ++ i)
    {
        Task * task = new Task(&sum, &mutexSum, TASK_ITER_COUNT);
        taskManager->Enqueue(task);
    }

    taskManager->Run();

    ASSERT_TRUE(sum == (1ll * TASK_COUNT * (0 + (TASK_ITER_COUNT - 1)) * TASK_ITER_COUNT / 2));
}

class TaskLauncher : public IRunnable
{
public:
    void Run()
    {
        mMutexTaskCnt->Lock();
        uint32_t taskCnt = *mTaskCnt;
        mMutexTaskCnt->Unlock();

        for (int i = 0; i < 100 && taskCnt > 0; ++ i, -- taskCnt)
        {
            Task * task = new Task(mSum, mMutexSum, TASK_ITER_COUNT);
            mTaskManager->Enqueue(task);
        }

        mMutexTaskCnt->Lock();
        {
            if ((*mTaskCnt = taskCnt) > 0)
            {
                TaskLauncher * taskLauncher = new TaskLauncher(mSum, mMutexSum, mTaskCnt, mMutexTaskCnt, mTaskManager);
                mTaskManager->Enqueue(taskLauncher);
            }
        }
        mMutexTaskCnt->Unlock();
    }

public:
    TaskLauncher(
        uint64_t * sum,
        Mutex * mutexSum,
        uint32_t * taskCnt,
        Mutex * mutexTaskCnt,
        ITaskManagerIn taskManager)
        : mTaskManager(taskManager)
        , mSum(sum)
        , mMutexSum(mutexSum)
        , mTaskCnt(taskCnt)
        , mMutexTaskCnt(mutexTaskCnt)
    {}

private:
    ITaskManagerRef     mTaskManager;
    uint64_t *          mSum;
    Mutex *             mMutexSum;
    uint32_t *          mTaskCnt;
    Mutex *             mMutexTaskCnt;
};

TEST(TaskManagerTests, LaunchTasks)
{
    ITaskManagerRef taskManager = ITaskManager::GetInstance();

    uint64_t sum = 0;
    Mutex mutexSum;

    uint32_t taskCnt = TASK_COUNT;
    Mutex mutexTaskCnt;

    TaskLauncher * taskLauncher = new TaskLauncher(&sum, &mutexSum, &taskCnt, &mutexTaskCnt, taskManager);
    taskManager->Enqueue(taskLauncher);

    taskManager->Run();

    ASSERT_TRUE(sum == (1ll * TASK_COUNT * (0 + (TASK_ITER_COUNT - 1)) * TASK_ITER_COUNT / 2));
}
