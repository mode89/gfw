#include "common/platform.h"
#include "common/critical_section.h"

#if PLATFORM_WIN32

    #include <windows.h>

    namespace Common {

        crit_sec_t CreateCriticalSection()
        {
            CRITICAL_SECTION * section = new CRITICAL_SECTION;
            ::InitializeCriticalSection(section);
            return section;
        }

        void DeleteCriticalSection(crit_sec_t section)
        {
            delete static_cast<CRITICAL_SECTION*>(section);
        }

        void EnterCriticalSection(crit_sec_t section)
        {
            ::EnterCriticalSection(static_cast<CRITICAL_SECTION *>(section));
        }

        void LeaveCriticalSection(crit_sec_t section)
        {
            ::LeaveCriticalSection(static_cast<CRITICAL_SECTION *>(section));
        }

    } // namespace Common

#else

    #error Unrecognized platform

#endif
