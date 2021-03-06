#ifndef __PROFILER_PROFILER_H__
#define __PROFILER_PROFILER_H__

#include "cmn/typedefs.h"
#include "cmn/platform.h"
#include "profiler/meson.h"
#include "profiler/token.h"

#if PROFILING

    // Define macro to produce function signature
    #if CMN_COMPILER_MSVC
        #define PROFILER_FUNC_SIG   __FUNCSIG__
    #elif CMN_COMPILER_MINGW
        #define PROFILER_FUNC_SIG   __PRETTY_FUNCTION__
    #endif

    #define PROFILER_CONCATENATE(x, y)  x ## y
    #define PROFILER_CONCATENATE2(x, y) PROFILER_CONCATENATE(x, y)
    #define PROFILER_TOKEN              PROFILER_CONCATENATE2(__Profiler_Token_, __LINE__)
    #define PROFILER_MESON              PROFILER_CONCATENATE2(__Profiler_Meson, __LINE__)

    #define PROFILE() \
        static Profiler::Token PROFILER_TOKEN(PROFILER_FUNC_SIG, __FILE__, __LINE__); \
        Profiler::Meson        PROFILER_MESON(&PROFILER_TOKEN);

#else // PROFILING

    #define PROFILE()

#endif // PROFILING

#endif // __PROFILER_PROFILER_H__
