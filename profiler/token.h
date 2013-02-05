#ifndef __PROFILER_TOKEN_H__
#define __PROFILER_TOKEN_H__

#include "common/typedefs.h"

namespace Profiler {

    class Token
    {
    public:
        Token(const char * name, const char * fileName, uint32_t lineNumber);

        inline
        uint32_t     GetHash() { return mHash; }

    private:
        const char * mName;
        const char * mFileName;
        uint32_t     mLineNumber;
        uint32_t     mHash;
    };

} // namespace Profiler

#endif // __RPOFILER_TOKEN_H__
