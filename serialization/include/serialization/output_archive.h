#ifndef __SERIALIZATION_OUTPUT_ARCHIVE_H__
#define __SERIALIZATION_OUTPUT_ARCHIVE_H__

#include "serialization/named_value.h"

namespace Serialization {

    template < class Stream >
    class OutputArchive
    {
    public:
        OutputArchive( Stream & stream )
            : mStream( stream )
        {

        }

        ~OutputArchive()
        {

        }

        template < class T > void
        operator & ( NamedValue<T> value )
        {

        }

    private:
        Stream &    mStream;
    };

} // namespace Serialization

#endif // __SERIALIZATION_OUTPUT_ARCHIVE_H__
