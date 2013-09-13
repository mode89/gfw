#ifndef __THREADPOOL_THREADPOOL_H__
#define __THREADPOOL_THREADPOOL_H__

#include "common/runnable.h"

namespace ThreadPool {

    AUTOREF_FORWARD_DECLARATION(IThreadPool);
    class IThreadPool : public Common::ARefCounted
    {
    public:
        static IThreadPoolRef
        GetInstance();

        virtual void
        Enqueue(Common::IRunnableIn) = 0;

        virtual void
        Run() = 0;

        virtual
        ~IThreadPool() {}
    };

} // namespace ThreadPool

#endif // __THREADPOOL_THREADPOOL_H__
