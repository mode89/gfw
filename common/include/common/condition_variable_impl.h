#ifndef __COMMON_CONDITION_VARIABLE_IMPL_H__
#define __COMMON_CONDITION_VARIABLE_IMPL_H__

#include <windows.h>

namespace Common {

    struct ConditionVariableImpl
    {
        CONDITION_VARIABLE nativeHandle;
    };

} // namespace Common

#endif // __COMMON_CONDITION_VARIABLE_IMPL_H__
