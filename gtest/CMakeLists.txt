project(GTEST)
cmake_minimum_required(VERSION 2.8)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()

add_library(gtest source/gtest-main.cc source/gtest-all.cc)
include_directories(include)
