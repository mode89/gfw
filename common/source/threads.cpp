#include "common/threads.h"
#include "common/trace.h"

#include <windows.h>

namespace Common {

    class Thread : public IThread
    {
    public:
        virtual void
        Join();

    public:
        Thread(IRunnableIn);
        ~Thread();

        bool
        Initialize();

    private:
        static DWORD WINAPI
        Proc(LPVOID data);

    private:
        HANDLE          mHandle;
        IRunnableRef    mRunnable;
    };

    IThreadRef CreateThread(IRunnableIn runnable)
    {
        TRACE_ASSERT(runnable.IsAttached());

        Thread * thread = new Thread(runnable);

        if (!thread->Initialize())
        {
            delete thread;
            return NULL;
        }

        return thread;
    }

    Thread::Thread(IRunnableIn runnable)
        : mHandle(NULL)
        , mRunnable(runnable)
    {

    }

    Thread::~Thread()
    {
        int res = 0;

        if (mHandle != NULL)
        {
            res = CloseHandle(mHandle);
            TRACE_ASSERT(res != 0);
        }
    }

    bool Thread::Initialize()
    {
        mHandle = ::CreateThread(NULL, 0, Thread::Proc, mRunnable.GetPointer(), 0, NULL);
        if (mHandle == NULL)
        {
            TRACE_ERROR("Failed to create a thread");
            return false;
        }

        return true;
    }

    DWORD WINAPI Thread::Proc(LPVOID data)
    {
        IRunnableRef runnable = static_cast<IRunnable*>(data);
        runnable->Run();
        return 0;
    }

    void Thread::Join()
    {
        WaitForSingleObject(mHandle, INFINITE);
    }

} // namespace Common
