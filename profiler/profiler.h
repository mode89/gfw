#ifndef __PROFILER_PROFILER_H__
#define __PROFILER_PROFILER_H__

#ifdef PROFILER

#define PROFILE_BEGIN(regionName)   Profiler::BeginRegion(#regionName)
#define PROFILE_END(regionName)     Profiler::EndRegion(#regionName)

#else // PROFILER

#define PROFILE_BEGIN(regionName)
#define PROFILE_END(regionName)

#endif // PROFILER



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

namespace Profiler {

    void BeginRegion(const char * regionName);
    void EndRegion(const char * regionName);

} // namespace Profiler

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RPFOILER_PROFILER_H__
