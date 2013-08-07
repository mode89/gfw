#ifndef __COMMON_FILE_H__
#define __COMMON_FILE_H__

#include "common\autoref.h"

namespace Common {

    AUTOREF_FORWARD_DECLARATION(File);
    class File: public ARefCounted
    {
    public:
        static FileRef          Create();

        virtual bool            Read(const char * filePath);

        inline
        virtual const void *    GetData() const     { return mData; }

        inline
        virtual uint64_t        GetSize() const     { return mSize; }

    public:
        File();

    private:
        char8_t *       mData;
        uint64_t        mSize;
    };

} // namespace Common

#endif // __COMMON_FILE_H__
