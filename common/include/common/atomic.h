#ifndef __COMMON_ATOMIC_H__
#define __COMMON_ATOMIC_H__

#include "common\typedefs.h"

namespace Common {

    void AtomicIncrement(int32_t &);
    void AtomicDecrement(int32_t &);
    void AtomicExchange(int32_t & dst, int32_t val);
    void AtomicCompareExchange(int32_t & dst, int32_t exch, int32_t cmp);

} // namespace Common

#endif // __COMMON_ATOMIC_H__
