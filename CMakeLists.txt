project(GLOBAL_SOLUTION)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DPROFILING=1)

include_directories(${GLOBAL_SOLUTION_SOURCE_DIR})

add_subdirectory(gtest)
add_subdirectory(irrlicht)

add_subdirectory(common)
add_subdirectory(profiler)
add_subdirectory(gfw)
