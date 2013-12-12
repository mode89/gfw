#ifndef __PROFILER_TOKEN_H__
#define __PROFILER_TOKEN_H__

#include "common/typedefs.h"

namespace Profiler {

    class Token
    {
    public:
        Token(const char * name, const char * fileName, uint32_t lineNumber);

        inline
        uint64_t        GetHash() const         { return mHash; }

        inline
        const char *    GetName() const         { return mName; }

        inline
        const char *    GetFileName() const     { return mFileName; }

        inline
        uint32_t        GetLineNumber() const   { return mLineNumber; }

    private:
        const char * mName;
        const char * mFileName;
        uint32_t     mLineNumber;
        uint64_t     mHash;
    };

} // namespace Profiler

#endif // __RPOFILER_TOKEN_H__
