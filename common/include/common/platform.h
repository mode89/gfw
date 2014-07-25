#ifndef __COMMON_PLATFORM_H__
#define __COMMON_PLATFORM_H__

#if defined(WIN32) || defined(__MINGW32__)
    #define PLAT_WIN32	1
#endif // defined(WIN32) || defined(__MINGW32__)

#if defined(__MINGW32__) || defined(__MINGW64__)
    #define PLAT_COMPILER_MINGW	1
#endif // defined(__MINGW32__) || defined(__MINGW64__)

#if defined(_MSC_VER)
    #define PLAT_COMPILER_MSVC	1
#endif // defined(_MSC_VER)

#if defined(NDEBUG)
    #define PLAT_NDEBUG 1
#else
    #define PLAT_DEBUG 1
#endif // defined(NDEBUG)

// Thread local storage
#if defined(PLAT_COMPILER_MSVC)
    #define PLAT_THREAD_LOCAL   __declspec(thread)
#elif defined(PLAT_COMPILER_MINGW)
    #define PLAT_THREAD_LOCAL   __thread
#endif

// Some compilers doesn't support noexcept() operator
#if defined( PLAT_COMPILER_MSVC )
    #define PLAT_NOEXCEPT( val )
#elif defined( PLAT_COMPILER_MINGW )
    #define PLAT_NOEXCEPT( val ) noexcept( val )
#endif

// If variable is unused use this macro
#define PLAT_UNUSED( var )  ( ( void ) var )

#define PLAT_STRINGIFY( val )   #val

// Overriding pragma
#if defined( PLAT_COMPILER_MSVC )
    #define PLAT_PRAGMA( dirv )     __pragma( dirv )
#elif defined( PLAT_COMPILER_MINGW )
    #define PLAT_PRAGMA( dirv )     _Pragma( #dirv )
#endif

// Controlling warnings
#if defined( PLAT_COMPILER_MSVC )
    #define PLAT_WARNING_PUSH                   PLAT_PRAGMA( warning( push ) )
    #define PLAT_WARNING_POP                    PLAT_PRAGMA( warning( pop ) )

    #define PLAT_WARNING_DEFAULT_MSVC( warn )   PLAT_PRAGMA( warning( default : warn ) )
    #define PLAT_WARNING_DISABLE_MSVC( warn )   PLAT_PRAGMA( warning( disable : warn ) )
    #define PLAT_WARNING_DISABLE_GCC( warn )
#elif defined( PLAT_COMPILER_MINGW )
    #define PLAT_WARNING_PUSH                   PLAT_PRAGMA( GCC diagnostic push )
    #define PLAT_WARNING_POP                    PLAT_PRAGMA( GCC diagnostic pop )

    #define PLAT_WARNING_DEFAULT_MSVC( warn )
    #define PLAT_WARNING_DISABLE_MSVC( warn )
    #define PLAT_WARNING_DISABLE_GCC( warn )    PLAT_PRAGMA( GCC diagnostic ignored "-W" #warn )
#endif

#endif // __COMMON_PLATFORM_H__
