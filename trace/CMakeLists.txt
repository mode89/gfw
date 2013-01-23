project(TRACE)
cmake_minimum_required(VERSION 2.8)

file(GLOB TRACE_SOURCE_FILES *.cpp)
add_library(trace ${TRACE_SOURCE_FILES})
