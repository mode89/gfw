#include "common/platform.h"
#include "common/critical_section.h"

#if PLATFORM_WIN32

    #include <windows.h>

    namespace Common {

        CriticalSection CreateCriticalSection()
        {
            CRITICAL_SECTION * section = new CRITICAL_SECTION;
            ::InitializeCriticalSection(section);
            return section;
        }

        void DeleteCriticalSection(CriticalSection section)
        {
            delete static_cast<CRITICAL_SECTION*>(section);
        }

        void EnterCriticalSection(CriticalSection section)
        {
            ::EnterCriticalSection(static_cast<CRITICAL_SECTION *>(section));
        }

        void LeaveCriticalSection(CriticalSection section)
        {
            ::LeaveCriticalSection(static_cast<CRITICAL_SECTION *>(section));
        }

    } // namespace Common

#else

    #error Unrecognized platform

#endif
