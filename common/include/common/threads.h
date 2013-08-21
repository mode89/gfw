#ifndef __COMMON_THREADS_H__
#define __COMMON_THREADS_H__

#include "common/autoref.h"

namespace Common {

    typedef uint32_t (*ThreadProc)(void *);

    class IThread : public ARefCounted
    {
    public:
        virtual void
        Join() = 0;

        virtual
        ~IThread() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IThread);

    IThreadRef
    CreateThread(ThreadProc, void * data);

} // namespace Common

#endif // __COMMON_THREADS_H__
