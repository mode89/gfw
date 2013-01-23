#include "gfw\graphics\opengl\platform.h"
#include "gfw\graphics\opengl\functions.h"

#include <windows.h>

namespace GFW { namespace OpenGL {

#if PLATFORM_WIN32

    static HMODULE sLibrary = NULL;

    uint32_t LoadFunctions()
    {
        if (sLibrary == NULL)
        {
            sLibrary = LoadLibrary("opengl32.dll");
        }

        return sLibrary != NULL;
    }

#else
#error Unrecognized platform
#endif // Determine OS

}} // namespace GFW::OpenGL
