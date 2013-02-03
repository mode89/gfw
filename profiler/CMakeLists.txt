project(PROFILER)
cmake_minimum_required(VERSION 2.8)

file(GLOB PROFILER_SOURCE_FILES *.cpp)
add_library(profiler ${PROFILER_SOURCE_FILES})
target_link_libraries(profiler common)
