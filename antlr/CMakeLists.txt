project(ANTLR)
cmake_minimum_required(VERSION 2.8)

include_directories(include/antlr)

file(GLOB ANTLR_SOURCE_FILES source/*.c)
add_library(antlr ${ANTLR_SOURCE_FILES})
