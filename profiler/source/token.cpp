#include "profiler/token.h"

namespace Profiler {

    Token::Token(const char * name, const char * fileName, uint32_t lineNumber)
        : mName(name)
        , mFileName(fileName)
        , mLineNumber(lineNumber)
    {
        mHash = reinterpret_cast<uint64_t>(this);
    }

} // namespace Profiler
