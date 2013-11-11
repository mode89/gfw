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

} // namespace Common
