#ifndef __SERIALIZATION_OUTPUT_ARCHIVE_H__
#define __SERIALIZATION_OUTPUT_ARCHIVE_H__

#include "common/autoref.h"
#include "serialization/named_value.h"

#include <stdlib.h>

namespace Serialization {

    using namespace Common;

    // ***************************************************************
    // Serialization of a value

    template < class Archive, class T > void
    Serialize( Archive & archive, T & value )
    {
        value.Serialize( archive );
    }

    template < class Archive, class T > void
    Serialize( Archive & archive, AutoRef<T> & value )
    {
        archive.SerializeArray( value.GetPointer(), 1 );
    }

    template < class Archive > void
    Serialize( Archive & archive, uint32_t & value )
    {
        archive.SerializeSimpleType( value );
    }

    template < class Archive > void
    Serialize( Archive & archive, int32_t & value )
    {
        archive.SerializeSimpleType( value );
    }

    template < class Archive > void
    Serialize( Archive & archive, uint8_t & value )
    {
        archive.SerializeSimpleType( value );
    }

    // ***************************************************************
    // Serialization of an array

    template < class Archive, class T > void
        Serialize( Archive & archive, T * ptr, uint32_t size )
    {
        for ( uint32_t i = 0; i < size; ++ i )
        {
            char num[16];
            _itoa( i, num, 10 );
            archive & CreateNamedValue( num, ptr[i] );
        }
    }

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
            Serialize( *this, value.GetValue() );
        }

        template < class T > void
        operator & ( NamedArray<T> value )
        {
            SerializeArray( value.GetValue(), value.GetSize() );
        }

        template < class T > void
        operator & ( NamedArray<AutoPointer<T>> array )
        {
            SerializeArray( array.GetValue().GetPointer(), array.GetSize() );
        }

        template < class T > void
        SerializeSimpleType( T value )
        {
            mStream.write( reinterpret_cast< const char * >( &value ), sizeof( T ) );
        }

        template < class T > void
        SerializeArray( T * arrayPtr, uint32_t size )
        {
            PointerIndex::iterator it = mPointerIndex.find( arrayPtr );
            if ( it == mPointerIndex.end() )
            {
                SerializeSimpleType( -1 ); // Indicates a new pointer

                uint32_t ptrIndex = mPointerIndex.size();
                mPointerIndex[ arrayPtr ] = ptrIndex;

                Serialize( *this, arrayPtr, size );
            }
            else
            {
                SerializeSimpleType( it->second );
            }
        }

    private:
        typedef std::unordered_map< const void *, uint32_t > PointerIndex;

        Stream &        mStream;
        PointerIndex    mPointerIndex;
    };

} // namespace Serialization

#endif // __SERIALIZATION_OUTPUT_ARCHIVE_H__
