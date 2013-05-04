#ifndef __COMMON_CRITICAL_SECTION_H__
#define __COMMON_CRITICAL_SECTION_H__

namespace Common {

    typedef void * CritSec;

    CritSec  CreateCriticalSection();

    void     DeleteCriticalSection(CritSec);

    void     EnterCriticalSection(CritSec);

    void     LeaveCriticalSection(CritSec);

} // namespace Common

#endif // __COMMON_CRITICAL_SECTION_H__
