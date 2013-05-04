#include "common/trace.h"

#include "common\file.h"
#include "common\platform.h"

#if PLATFORM_WIN32
#include <windows.h>
#else
#error Unrecognized platform
#endif

namespace Common {

    FileRef File::Create()
    {
        return new File;
    }

    File::File()
        : mData(NULL)
        , mSize(0)
    {
    }

    uint32_t File::Read( const char * filePath )
    {
        HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        TRACE_ASSERT(hFile != INVALID_HANDLE_VALUE);

        if (hFile != INVALID_HANDLE_VALUE)
        {
            BOOL res = 0;

            DWORD size = GetFileSize(hFile, NULL);
            TRACE_ASSERT(size != NULL);

            if (size != 0)
            {
                void * data = new char8_t [size + 1];
                TRACE_ASSERT(data != NULL);

                if (data != NULL)
                {
                    DWORD rb = 0;
                    res = ReadFile(hFile, data, size, &rb, NULL);
                    TRACE_ASSERT(res != NULL);
                    TRACE_ASSERT(rb == size);

                    if (res != 0 && rb == size)
                    {
                        mSize = size;
                        mData = static_cast<char8_t*>(data);
                        mData[mSize] = 0;

                        return 1;
                    }
                }
            }

            res = CloseHandle(hFile);
            TRACE_ASSERT(res != NULL);
        }

        return 0;
    }

} // namespace Common
