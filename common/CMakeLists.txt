project(COMMON)
cmake_minimum_required(VERSION 2.8)

file(GLOB COMMON_SOURCE_FILES source/*.cpp)
source_group("Source" FILES ${COMMON_SOURCE_FILES})

file(GLOB COMMON_HEADER_FILES include/common/*.h)
source_group("Include" FILES ${COMMON_HEADER_FILES})

add_library(common ${COMMON_SOURCE_FILES} ${COMMON_HEADER_FILES})
include_directories(include)
