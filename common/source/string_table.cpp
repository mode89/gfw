#include "common/string_table.h"

#include <string.h>

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

    const char * StringTable::Resolve(const char * name)
    {
        char * retVal = NULL;

        StringMap::iterator it = mMap.find(name);
        if (it != mMap.end())
        {
            retVal = it->second;
        }
        else
        {
            retVal = new char [strlen(name) + 1];
            strcpy(retVal, name);
            mMap[retVal] = retVal;
        }

        return retVal;
    }

} // namespace Common
