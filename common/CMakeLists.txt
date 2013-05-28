project(COMMON)
cmake_minimum_required(VERSION 2.8)

file(GLOB COMMON_SOURCE_FILES source/*.cpp)
add_library(common ${COMMON_SOURCE_FILES})
include_directories(include)

add_subdirectory(tests)
