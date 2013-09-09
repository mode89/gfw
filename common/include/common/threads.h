#ifndef __COMMON_THREADS_H__
#define __COMMON_THREADS_H__

#include "common/runnable.h"

namespace Common {

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
    CreateThread(IRunnableIn);

} // namespace Common

#endif // __COMMON_THREADS_H__
