#ifndef __COMMON_CRITICAL_SECTION_H__
#define __COMMON_CRITICAL_SECTION_H__

namespace Common {

    typedef void * crit_sec_t;

    crit_sec_t  CreateCriticalSection();

    void        DeleteCriticalSection(crit_sec_t);

    void        EnterCriticalSection(crit_sec_t);

    void        LeaveCriticalSection(crit_sec_t);

} // namespace Common

#endif // __COMMON_CRITICAL_SECTION_H__
