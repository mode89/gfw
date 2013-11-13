#ifndef __SERIALIZATION_INPUT_ARCHIVE_H__
#define __SERIALIZATION_INPUT_ARCHIVE_H__

#include "common/trace.h"
#include "serialization/named_value.h"
#include "serialization/serialize.h"

namespace Serialization {

    template < class Stream >
    class InputArchive
    {
    public:
        InputArchive( Stream & stream )
            : mStream( stream )
        {
            // Define a NULL pointer
            mPointerIndex.push_back( NULL );
        }

        template < class T > void
        operator & ( const NamedValue<T> & value )
        {
            Serialize( *this, &value.GetValue(), 1 );
        }

        template < class T > void
        operator & ( const NamedArray<T> & array )
        {
            Serialize( *this, array.GetValue(), array.GetSize() );
        }

        template < class T > void
        SerializeSimpleType( T * value, uint32_t size )
        {
            mStream.read( reinterpret_cast<char*>( value ), size * sizeof( T ) );
        }

        template < class T > void
        SerializePointer( T* & ptr, uint32_t size = 0 ) // If size > 0 then allocate an array, otherwise - single object
        {
            uint32_t ptrIndex = static_cast<uint32_t>( -1 );
            SerializeSimpleType( &ptrIndex, 1 );

            if ( ptrIndex == static_cast<uint32_t>( -1 ) )
            {
                ptr = size ? new T [size] : new T; // If size > 0 then treat as an array

                mPointerIndex.push_back( ptr );

                Serialize( *this, ptr, size ? size : 1 );
            }
            else
            {
                TRACE_ASSERT( ptrIndex < mPointerIndex.size() );
                ptr = static_cast<T*>( mPointerIndex[ ptrIndex ] );
            }
        }

        template < class T > void
        SerializeAutoRef( AutoRef<T> & autoRef )
        {
            T * ptr = NULL;
            SerializePointer( ptr );
            autoRef = ptr;
        }

        template < class T > void
        SerializeAutoArray( AutoArray<T> & autoPtr, uint32_t size )
        {
            T * ptr = NULL;
            SerializePointer( ptr, size );
            autoPtr = ptr;
        }

    private:
        typedef std::vector< void * > PointerIndex;

        Stream &        mStream;
        PointerIndex    mPointerIndex;
    };

} // namespace Serialization

#endif // __SERIALIZATION_INPUT_ARCHIVE_H__
