project(COMMON)
cmake_minimum_required(VERSION 2.8)

file(GLOB COMMON_SOURCE_FILES *.cpp)
add_library(common ${COMMON_SOURCE_FILES})

add_subdirectory(tests)
