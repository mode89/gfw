#if defined( EXCEPTION_NAMESPACE ) && defined( EXCEPTION_CODES )

    #define STRINGIFY( val ) #val

    #define C( code, message ) \
        class code : public std::exception \
        { \
        public: \
            template < typename ... Args > \
            code( Args ... args ) { \
                char buffer[ 4096 ]; \
                std::sprintf( buffer, message, args... ); \
                mWhat = buffer; \
            } \
            const char * what() const throw() { \
                return mWhat.c_str(); \
            } \
        private: \
            std::string mWhat; \
        }; \

    namespace EXCEPTION_NAMESPACE {
        EXCEPTION_CODES
    };

    #undef STRINGIFY
    #undef C

#else // defined( EXCEPTION_NAMESPACE ) && defined( EXCEPTION_CODES )

    #if !defined( EXCEPTION_NAMESPACE )
        #error EXCEPTION_NAMESPACE is undefined
    #endif

    #if !defined( EXCEPTION_CODES )
        #error EXCEPTION_CODES is undefined
    #endif

#endif // defined( EXCEPTION_NAMESPACE ) && defined( EXCEPTION_CODES )