project(GFW_OPENGL)
cmake_minimum_required(VERSION 2.8)

file(GLOB OPENGL_SOURCE_FILES *.cpp)
add_library(gfw_opengl ${OPENGL_SOURCE_FILES})
target_link_libraries(gfw_opengl gfw)
