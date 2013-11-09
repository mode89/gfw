#ifndef __SERIALIZATION_SERIALIZE_H__
#define __SERIALIZATION_SERIALIZE_H__

#include "common/autoref.h"
#include "common/typedefs.h"

namespace Serialization {

    using namespace Common;

    template < class Archive, class T > void
    Serialize( Archive & archive, T & value )
    {
        value.Serialize( archive );
    }

    template < class Archive, class T > void
    Serialize( Archive & archive, AutoRef<T> & value )
    {
        archive.SerializeAutoRef( value );
    }

    template < class Archive, class T > void
    Serialize( Archive & archive, T * ptr, uint32_t size )
    {
        for ( uint32_t i = 0; i < size; ++ i )
        {
            Serialize( archive, ptr[i] );
        }
    }

    template < class Archive, class T > void
    Serialize( Archive & archive, AutoArray<T> & ptr, uint32_t size )
    {
        archive.SerializeAutoArray( ptr, size );
    }

    template < class Archive > void
    Serialize( Archive & archive, uint32_t * ptr, uint32_t size )
    {
        archive.SerializeSimpleType( ptr, size );
    }

    template < class Archive > void
    Serialize( Archive & archive, int32_t * ptr, uint32_t size )
    {
        archive.SerializeSimpleType( ptr, size );
    }

    template < class Archive > void
    Serialize( Archive & archive, uint8_t * ptr, uint32_t size )
    {
        archive.SerializeSimpleType( ptr, size );
    }

} // namespace Serialization

#endif // __SERIALIZATION_SERIALIZE_H__
