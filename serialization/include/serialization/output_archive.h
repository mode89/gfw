#ifndef __SERIALIZATION_OUTPUT_ARCHIVE_H__
#define __SERIALIZATION_OUTPUT_ARCHIVE_H__

#include "common/autoref.h"
#include "serialization/named_value.h"
#include "serialization/serialize.h"

#include <stdlib.h>
#include <unordered_map>

namespace Serialization {

    using namespace Common;

    template < class Stream >
    class OutputArchive
    {
    public:
        OutputArchive( Stream & stream )
            : mStream( stream )
        {
            // Define a NULL pointer
            mPointerIndex[ NULL ] = 0;
        }

        ~OutputArchive()
        {

        }

        template < class T > void
        operator & ( NamedValue<T> value )
        {
            Serialize( *this, &value.GetValue(), 1 );
        }

        template < class T > void
        operator & ( NamedArray<T> value )
        {
            Serialize( *this, value.GetValue(), value.GetSize() );
        }

        template < class T > void
        SerializeSimpleType( T * value, uint32_t size )
        {
            mStream.write( reinterpret_cast< const char * >( value ), size * sizeof( T ) );
        }

        template < class T > void
        SerializePointer( T * arrayPtr, uint32_t size )
        {
            PointerIndex::iterator it = mPointerIndex.find( arrayPtr );
            if ( it == mPointerIndex.end() )
            {
                uint32_t ptrIndex = -1;
                SerializeSimpleType( &ptrIndex, 1 ); // Indicates a new pointer

                ptrIndex = mPointerIndex.size();
                mPointerIndex[ arrayPtr ] = ptrIndex;

                Serialize( *this, arrayPtr, size );
            }
            else
            {
                SerializeSimpleType( &it->second, 1 );
            }
        }

        template < class T > void
        SerializeAutoRef( AutoRef<T> & autoRef )
        {
            SerializePointer( autoRef.GetPointer(), 1 );
        }

        template < class T > void
        SerializeAutoArray( AutoArray<T> & autoPtr, uint32_t size )
        {
            SerializePointer( autoPtr.GetPointer(), size );
        }

    private:
        typedef std::unordered_map< const void *, uint32_t > PointerIndex;

        Stream &        mStream;
        PointerIndex    mPointerIndex;
    };

} // namespace Serialization

#endif // __SERIALIZATION_OUTPUT_ARCHIVE_H__
