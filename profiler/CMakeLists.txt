project(PROFILER)
cmake_minimum_required(VERSION 2.8)

file(GLOB PROFILER_SOURCE_FILES source/*.cpp)
source_group("Source" FILES ${PROFILER_SOURCE_FILES})

file(GLOB PROFILER_HEADER_FILES include/profiler/*.h)
source_group("Include" FILES ${PROFILER_HEADER_FILES})

add_library(profiler ${PROFILER_SOURCE_FILES} ${PROFILER_HEADER_FILES})
include_directories(include)

find_package(common CONFIG)
target_link_libraries(profiler ${COMMON_LIBS})
include_directories(${COMMON_INCLUDE_DIRS})
