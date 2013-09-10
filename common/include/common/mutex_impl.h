#ifndef __COMMON_MUTEX_IMPL_H__
#define __COMMON_MUTEX_IMPL_H__

#include <windows.h>

namespace Common {

    struct MutexImpl
    {
        CRITICAL_SECTION nativeHandle;
    };

} // namespace Common

#endif // __COMMON_MUTEX_IMPL_H__
