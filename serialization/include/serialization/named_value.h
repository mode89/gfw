#ifndef __SERIALIZATION_NAMED_VALUE_H__
#define __SERIALIZATION_NAMED_VALUE_H__

#define NAMED_VALUE( value )          Serialization::CreateNamedValue( #value, value )
#define NAMED_ARRAY( value, size )    Serialization::CreateNamedArray( #value, value, size )

namespace Serialization {

    template < class T >
    class NamedValue
    {
    public:
        NamedValue( const char * name, T & value )
            : mName( name )
            , mValue( value )
        {}

        inline const char *
        GetName() { return mName; }

        inline T &
        GetValue() { return mValue; }

    protected:
        const char *    mName;
        T &             mValue;
    };

    template < class T >
    NamedValue<T> CreateNamedValue( const char * name, T & value )
    {
        return NamedValue<T>( name, value );
    }

    template < class T >
    class NamedArray : public NamedValue<T>
    {
    public:
        NamedArray( const char * name, T & a, uint32_t size )
            : NamedValue<T>( name, a )
            , mSize( size )
        {}

        inline uint32_t
        GetSize() { return mSize; }

    private:
        uint32_t    mSize;
    };

    template < class T >
    NamedArray<T> CreateNamedArray( const char * name, T & a, uint32_t size )
    {
        return NamedArray<T>( name, a, size );
    }

} // namespace Serialization

#endif // __SERIALIZATION_NAMED_VALUE_H__
