#ifndef __COMMON_RUNNABLE_H__
#define __COMMON_RUNNABLE_H__

#include "common/autoref.h"

namespace Common {

    class IRunnable : public Common::ARefCounted
    {
    public:
        virtual void
        Run() = 0;

        virtual
        ~IRunnable() {}
    };
    AUTOREF_REFERENCE_DECLARATION(IRunnable);

} // namespace Common

#endif // __COMMON_RUNNABLE_H__
