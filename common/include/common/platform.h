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

#endif // __COMMON_PLATFORM_H__
