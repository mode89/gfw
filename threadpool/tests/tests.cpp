#include "gtest/gtest.h"

#include "common/mutex.h"
#include "threadpool/threadpool.h"

#define TASK_COUNT          100000
#define TASK_ITER_COUNT     3000

using namespace Common;
using namespace ThreadPool;

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

TEST(ThreadPoolTests, CreateAndRunTasks)
{
    IThreadPoolRef threadPool = IThreadPool::GetInstance();

    Common::Mutex mutexSum;
    uint64_t sum = 0;

    for (int i = 0; i < TASK_COUNT; ++ i)
    {
        Task * task = new Task(&sum, &mutexSum, TASK_ITER_COUNT);
        threadPool->Enqueue(task);
    }

    threadPool->Run();

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
            mThreadPool->Enqueue(task);
        }

        mMutexTaskCnt->Lock();
        {
            if ((*mTaskCnt = taskCnt) > 0)
            {
                TaskLauncher * taskLauncher = new TaskLauncher(mSum, mMutexSum, mTaskCnt, mMutexTaskCnt, mThreadPool);
                mThreadPool->Enqueue(taskLauncher);
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
        IThreadPoolIn threadPool)
        : mThreadPool(threadPool)
        , mSum(sum)
        , mMutexSum(mutexSum)
        , mTaskCnt(taskCnt)
        , mMutexTaskCnt(mutexTaskCnt)
    {}

private:
    IThreadPoolRef      mThreadPool;
    uint64_t *          mSum;
    Mutex *             mMutexSum;
    uint32_t *          mTaskCnt;
    Mutex *             mMutexTaskCnt;
};

TEST(ThreadPoolTests, LaunchTasks)
{
    IThreadPoolRef threadPool = IThreadPool::GetInstance();

    uint64_t sum = 0;
    Mutex mutexSum;

    uint32_t taskCnt = TASK_COUNT;
    Mutex mutexTaskCnt;

    TaskLauncher * taskLauncher = new TaskLauncher(&sum, &mutexSum, &taskCnt, &mutexTaskCnt, threadPool);
    threadPool->Enqueue(taskLauncher);

    threadPool->Run();

    ASSERT_TRUE(sum == (1ll * TASK_COUNT * (0 + (TASK_ITER_COUNT - 1)) * TASK_ITER_COUNT / 2));
}
