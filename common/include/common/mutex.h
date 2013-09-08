#ifndef __COMMON_FUTEX_H__
#define __COMMON_FUTEX_H__

#include "common/critical_section.h"

namespace Common {

    class Futex
    {
    public:
        inline void Lock()
        {
            EnterCriticalSection(mCriticalSection);
        }

        inline void Unlock()
        {
            LeaveCriticalSection(mCriticalSection);
        }

    public:
        Futex()
            : mCriticalSection(NULL)
        {
            mCriticalSection = CreateCriticalSection();
        }

        ~Futex()
        {
            DeleteCriticalSection(mCriticalSection);
        }

    private:
        CriticalSection     mCriticalSection;
    };

} // namespace Common

#endif // __COMMON_FUTEX_H__
