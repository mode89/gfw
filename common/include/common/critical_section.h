#ifndef __COMMON_CRITICAL_SECTION_H__
#define __COMMON_CRITICAL_SECTION_H__

namespace Common {

    typedef void * CriticalSection;

    CriticalSection  CreateCriticalSection();

    void     DeleteCriticalSection(CriticalSection);

    void     EnterCriticalSection(CriticalSection);

    void     LeaveCriticalSection(CriticalSection);

} // namespace Common

#endif // __COMMON_CRITICAL_SECTION_H__
