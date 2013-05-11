project(GLOBAL_SOLUTION)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DPROFILING=1)

if (MSVC)
    # Enable multithreaded compiling
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

include_directories(${GLOBAL_SOLUTION_SOURCE_DIR})

add_subdirectory(gtest)
add_subdirectory(irrlicht)

add_subdirectory(common)
add_subdirectory(profiler)
add_subdirectory(gfw)
