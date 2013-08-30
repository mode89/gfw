project(GTEST)
cmake_minimum_required(VERSION 2.8)

add_library(gtest source/gtest-main.cc source/gtest-all.cc)
include_directories(include)
