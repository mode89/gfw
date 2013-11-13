#include "common/crc32.h"
#include "common/string_table.h"
#include "common/trace.h"

#include <cstring>

namespace Common {

    StringTable::StringTable()
    {

    }

    StringTable::~StringTable()
    {
        for (StringMap::iterator it = mMap.begin(); it != mMap.end(); ++ it)
        {
            delete [] it->second;
        }
    }

    InternedString StringTable::Resolve( const char * name )
    {
        char * retVal = NULL;

        uint32_t hash = CRC32( name );

        StringMap::iterator it = mMap.find( hash );
        if (it != mMap.end())
        {
            retVal = it->second;
        }
        else
        {
            retVal = new char [strlen(name) + 1];
            strcpy(retVal, name);
            mMap[ hash ] = retVal;
        }

        return InternedString( retVal, hash );
    }

    InternedString StringTable::Resolve( const InternedStringBinary & string )
    {
        char * retVal = NULL;
        uint32_t hash = string.GetHash();

        StringMap::iterator it = mMap.find( hash );
        TRACE_ASSERT( it != mMap.end() );
        if ( it != mMap.end() )
        {
            retVal = it->second;
        }

        return InternedString( retVal, hash );
    }

    void StringTable::Resolve( StringTableBinaryRef bin )
    {
        const uint8_t * stringListEnd = bin->mStringList + bin->mStringListSize;
        const uint8_t * string = bin->mStringList;
        while ( string != stringListEnd )
        {
            Resolve( reinterpret_cast<const char*>( string ) );

            // Advance to the next string
            while ( *string ) string ++;
            string ++;
        }
    }

    StringTable::operator StringTableBinaryRef()
    {
        // Calculate string list size
        uint32_t stringListSize = 0;
        for ( StringTable::StringMap::iterator it = mMap.begin(); it != mMap.end(); ++ it )
        {
            stringListSize += ( strlen( it->second ) + 1 );
        }

        // Construct the string list
        uint8_t * stringList = new uint8_t [ stringListSize ];
        uint8_t * ptr = stringList;
        for ( StringTable::StringMap::iterator it = mMap.begin(); it != mMap.end(); ++ it )
        {
            strcpy( reinterpret_cast<char*>( ptr ), it->second );
            ptr += ( strlen( it->second ) + 1 );
        }

        StringTableBinaryRef bin = new StringTableBinary;
        bin->mStringList     = stringList;
        bin->mStringListSize = stringListSize;

        return bin;
    }

} // namespace Common
