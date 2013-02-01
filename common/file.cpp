#include "common\file.h"
#include "common\platform.h"
#include "common\trace.h"

#if PLATFORM_WIN32
#include <windows.h>
#else
#error Unrecognized platform
#endif

namespace Common {

    FileRef File::Create()
    {
        IAllocator * a = GetDefaultAllocator();
        return new(a) File(a);
    }

    File::File(IAllocator * a)
        : ADeallocatable(a)
        , mData(NULL)
        , mSize(0)
    {
        
    }

    File::~File()
    {
        mAllocator->Free(mData);
    }

    uint32_t File::Read( const char * filePath )
    {
        HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
        TRACE_ASSERT(hFile != INVALID_HANDLE_VALUE);

        if (hFile != INVALID_HANDLE_VALUE)
        {
            BOOL res = 0;

            DWORD size = GetFileSize(hFile, NULL);
            TRACE_ASSERT(size != NULL);

            if (size != 0)
            {
                void * data = mAllocator->Alloc(size);
                TRACE_ASSERT(data != NULL);

                if (data != NULL)
                {
                    DWORD rb = 0;
                    res = ReadFile(hFile, data, size, &rb, NULL);
                    TRACE_ASSERT(res != NULL);
                    TRACE_ASSERT(rb == size);

                    if (res != NULL && rb == size)
                    {
                        mSize = size;
                        mData = data;

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
