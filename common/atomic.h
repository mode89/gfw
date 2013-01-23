#ifndef __COMMON_ATOMIC_H__
#define __COMMON_ATOMIC_H__

#include "common\typedefs.h"

namespace Common {

    void AtomicIncrement(int32_t &);
    void AtomicDecrement(int32_t &);

} // namespace Common

#endif // __COMMON_ATOMIC_H__
