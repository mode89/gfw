#include "common/threads.h"
#include "common/trace.h"

#include <windows.h>

namespace Common {

    static void SetThreadName(const char * name)
    {
        if (name == NULL) return;

#if defined(PLAT_DEBUG) && defined(PLAT_COMPILER_MSVC)
        #define MS_VC_EXCEPTION 0x406d1388

        struct THREADNAME_INFO
        {
            DWORD dwType;        // must be 0x1000
            LPCSTR szName;       // pointer to name (in same addr space)
            DWORD dwThreadID;    // thread ID (-1 caller thread)
            DWORD dwFlags;       // reserved for future use, most be zero
        };

        THREADNAME_INFO info;
        info.dwType     = 0x1000;
        info.szName     = name;
        info.dwThreadID = -1;
        info.dwFlags    = 0;

        __try
        {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD),
                (DWORD *)&info);
        }
        __except (EXCEPTION_CONTINUE_EXECUTION) {}
#endif // PLAT_DEBUG && PLAT_COMPILER_MSVC
    }

    class Thread : public IThread
    {
    public:
        virtual void
        Join();

    public:
        Thread(IRunnableIn, const char * debugName);
        ~Thread();

        bool
        Initialize();

        const char *
        GetName() { return mName; }

    private:
        static DWORD WINAPI
        Proc(LPVOID data);

    private:
        HANDLE          mHandle;
        IRunnableRef    mRunnable;
        char            mName[128];
    };

    IThreadRef CreateThread(IRunnableIn runnable, const char * debugName /* = NULL */ )
    {
        TRACE_ASSERT(runnable.IsAttached());

        Thread * thread = new Thread(runnable, debugName);

        if (!thread->Initialize())
        {
            delete thread;
            return NULL;
        }

        return thread;
    }

    Thread::Thread(IRunnableIn runnable, const char * debugName)
        : mHandle(NULL)
        , mRunnable(runnable)
    {
        if (debugName != NULL)
        {
            strcpy(mName, debugName);
        }
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
            TRACE_ERROR("Failed to create a thread");
            return false;
        }

        return true;
    }

    DWORD WINAPI Thread::Proc(LPVOID data)
    {
        Thread * thread = static_cast<Thread*>(data);
        SetThreadName(thread->mName);
        thread->mRunnable->Run();
        return 0;
    }

    void Thread::Join()
    {
        WaitForSingleObject(mHandle, INFINITE);
    }

} // namespace Common
