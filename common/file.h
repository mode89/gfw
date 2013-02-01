#ifndef __COMMON_FILE_H__
#define __COMMON_FILE_H__

#include "common\autoref.h"
#include "common\allocator.h"

namespace Common {

    AUTOREF_FORWARD_DECLARATION(File);
    class File: public ADeallocatable, public AutoRefObject
    {
    public:
        static FileRef          Create();

        virtual uint32_t        Read(const char * filePath);

        inline
        virtual const void *    GetData() const     { return mData; }

        inline
        virtual uint64_t        GetSize() const     { return mSize; }

    public:
        File(IAllocator * a);
        ~File();

    protected:
        inline
        virtual void *          GetImplThis()       { return this; }

    private:
        void *          mData;
        uint64_t        mSize;
    };

} // namespace Common

#endif // __COMMON_FILE_H__
