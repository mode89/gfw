#ifndef __THREADPOOL_WIN_THREADPOOL_H__
#define __THREADPOOL_WIN_THREADPOOL_H__

#include "common/autoref.h"
#include "common/condition_variable.h"
#include "common/mutex.h"
#include "threadpool/threadpool.h"

#include <queue>

namespace ThreadPool {

    class ThreadPoolImpl : public IThreadPool
    {
    public:
        virtual void
        Enqueue(Common::IRunnableIn);

        virtual void
        Run();

    public:
        Common::IRunnableRef
        Dequeue();

    public:
        ThreadPoolImpl();
        ~ThreadPoolImpl();

    private:
        static ThreadPoolImpl *             mInstance;

        uint32_t                            mThreadCount;

        std::queue<Common::IRunnableRef>    mQueue;
        Common::Mutex                       mMutexQueue;

        Common::ConditionVariable           mNotEmptyCondition;
        uint32_t                            mWaitersCount;

        friend class IThreadPool;
    };
    AUTOREF_REFERENCE_DECLARATION(ThreadPoolImpl);

} // namespace ThreadPool

#endif // __THREADPOOL_WIN_THREADPOOL_H__
