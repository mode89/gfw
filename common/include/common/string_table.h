#ifndef __COMMON_STRING_TABLE_H__
#define __COMMON_STRING_TABLE_H__

#include "common/autoref.h"
#include <unordered_map>

namespace Common {

    class StringTable : public Common::ARefCounted
    {
    public:
        const char *
        Resolve(const char * name);

    public:
        StringTable();
        ~StringTable();

    private:
        typedef std::unordered_map < const char *, char * >  StringMap;

        StringMap   mMap;
    };
    AUTOREF_REFERENCE_DECLARATION(StringTable);

} // namespace Common

#endif // __COMMON_STRING_TABLE_H__
