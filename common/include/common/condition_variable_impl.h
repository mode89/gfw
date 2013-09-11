#ifndef __COMMON_CONDITION_VARIABLE_IMPL_H__
#define __COMMON_CONDITION_VARIABLE_IMPL_H__

#include "common/platform.h"
#include "common/typedefs.h"

#ifdef PLAT_COMPILER_MSVC

#include <windows.h>

namespace Common {

    struct ConditionVariableImpl
    {
        CONDITION_VARIABLE nativeHandle;
    };

} // namespace Common

#elif defined(PLAT_COMPILER_MINGW)

#include <windows.h>

namespace Common {

    struct ConditionVariableImpl
    {
        HANDLE              nativeHandle;
        uint32_t            waitersCount;
        CRITICAL_SECTION    mutexWaitersCount;
    };

} // namespace Common

#else
#error Undefined platform
#endif

#endif // __COMMON_CONDITION_VARIABLE_IMPL_H__
