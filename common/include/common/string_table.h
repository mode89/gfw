#ifndef __COMMON_STRING_TABLE_H__
#define __COMMON_STRING_TABLE_H__

#include "common/autoref.h"
#include "serialization/named_value.h"

#include <unordered_map>

namespace Common {

    class InternedString
    {
    public:
        const char *
        GetString() const { return mString; }

        uint32_t
        GetHash() const { return mHash; }

    public:
        InternedString()
            : mString( NULL )
            , mHash( 0 )
        {}

        InternedString( const char * string, uint32_t hash )
            : mString( string )
            , mHash( hash )
        {}

    private:
        const char *    mString;
        uint32_t        mHash;
    };

    class InternedStringBinary
    {
    public:
        uint32_t
        GetHash() const { return mHash; }

    public:
        InternedStringBinary()
            : mHash( 0 )
        {}

        InternedStringBinary( const InternedString & string )
            : mHash( string.GetHash() )
        {}

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( mHash );
        }

    private:
        uint32_t    mHash;
    };

    class StringTable : public Common::ARefCounted
    {
    public:
        InternedString
        Resolve( const char * name );

        InternedString
        Resolve( const InternedStringBinary & string );

    public:
        StringTable();
        ~StringTable();

    private:
        typedef std::unordered_map < uint32_t, char * >  StringMap;

        StringMap   mMap;

        friend class StringTableBinary;
    };
    AUTOREF_REFERENCE_DECLARATION( StringTable );

    class StringTableBinary : public Common::ARefCounted
    {
    public:
        StringTableBinary();
        StringTableBinary( StringTableRef );

        template < class Archive > void
        Serialize( Archive & archive )
        {
            archive & NAMED_VALUE( mStringListSize );
            archive & NAMED_ARRAY( mStringList, mStringListSize );
        }

    private:
        typedef Common::AutoArray< uint8_t > ByteArray;

        ByteArray   mStringList;
        uint32_t    mStringListSize;
    };
    AUTOREF_REFERENCE_DECLARATION( StringTableBinary );

} // namespace Common

#endif // __COMMON_STRING_TABLE_H__
