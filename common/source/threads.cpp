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
        Thread(ThreadProc, void * data);
        ~Thread();

        bool
        Initialize();

    private:
        static DWORD WINAPI
        Proc(LPVOID data);

    private:
        HANDLE      mHandle;

        ThreadProc  mProc;
        void *      mData;
    };

    IThreadRef CreateThread(ThreadProc proc, void * data)
    {
        TRACE_ASSERT(proc != NULL);

        Thread * thread = new Thread(proc, data);

        if (!thread->Initialize())
        {
            delete thread;
            return NULL;
        }

        return thread;
    }

    Thread::Thread(ThreadProc proc, void * data)
        : mHandle(NULL)
        , mProc(proc)
        , mData(data)
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
        mHandle = ::CreateThread(NULL, 0, Thread::Proc, this, 0, NULL);
        if (mHandle == NULL)
        {
            return false;
        }

        return true;
    }

    DWORD WINAPI Thread::Proc(LPVOID data)
    {
        Thread * thread = static_cast<Thread*>(data);
        return thread->mProc(thread->mData);
    }

    void Thread::Join()
    {
        WaitForSingleObject(mHandle, INFINITE);
    }

} // namespace Common
