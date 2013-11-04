#ifndef __SERIALIZATION_NAMED_VALUE_H__
#define __SERIALIZATION_NAMED_VALUE_H__

#define NAMED_VALUE( name, value ) Serialization::CreateNamedValue( name, value )

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

    private:
        const char *    mName;
        T &             mValue;
    };

    template < class T >
    NamedValue<T> CreateNamedValue( const char * name, T & value )
    {
        return NamedValue<T>( name, value );
    }

} // namespace Serialization

#endif // __SERIALIZATION_NAMED_VALUE_H__
