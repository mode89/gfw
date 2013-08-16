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

#if defined(_DEBUG)
    #define PLAT_DEBUG  1
#endif // defined(_DEBUG)

#if defined(NDEBUG)
    #define PLAT_NDEBUG 1
#endif // defined(NDEBUG)

#endif // __COMMON_PLATFORM_H__
