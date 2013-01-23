project(GFW)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(graphics/opengl)
add_subdirectory(platform)

add_library(gfw *.cpp)
target_link_libraries(gfw gfw_platform gfw_opengl common trace)

add_subdirectory(tests)
