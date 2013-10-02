project(MASTER)
cmake_minimum_required(VERSION 2.8)

# Include common utils

include(cmake/common.cmake)

# Add necessary projects to the build tree

add_subdirectory(antlr)
add_subdirectory(common)
add_subdirectory(gfw)
add_subdirectory(gfw_fxc)
add_subdirectory(gtest)
add_subdirectory(profiler)
add_subdirectory(ripples)
add_subdirectory(threadpool)
