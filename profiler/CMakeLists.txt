project(PROFILER)
cmake_minimum_required(VERSION 2.8)

file(GLOB PROFILER_SOURCE_FILES source/*.cpp)
add_library(profiler ${PROFILER_SOURCE_FILES})
include_directories(include)

find_package(common CONFIG)
target_link_libraries(profiler ${COMMON_LIBRARIES})
include_directories(${COMMON_INCLUDE_DIRS})
