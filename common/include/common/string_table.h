#ifndef __COMMON_STRING_TABLE_H__
#define __COMMON_STRING_TABLE_H__

#include "common/autoref.h"
#include <unordered_map>

namespace Common {

    class InternedString
    {
    public:
        const char *
        GetString() { return mString; }

        uint32_t
        GetHash() { return mHash; }

    public:
        InternedString( const char * string, uint32_t hash )
            : mString( string )
            , mHash( hash )
        {}

    private:
        const char *    mString;
        uint32_t        mHash;
    };

    class StringTable : public Common::ARefCounted
    {
    public:
        InternedString
        Resolve( const char * name );

    public:
        StringTable();
        ~StringTable();

    private:
        typedef std::unordered_map < uint32_t, char * >  StringMap;

        StringMap   mMap;
    };
    AUTOREF_REFERENCE_DECLARATION(StringTable);

} // namespace Common

#endif // __COMMON_STRING_TABLE_H__
