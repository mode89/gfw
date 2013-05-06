project(GFW)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(graphics/opengl)

file(GLOB SOURCE_FILES *.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})
file(GLOB SOURCE_FILES common/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})
file(GLOB SOURCE_FILES graphics/common/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})

add_library(gfw ${GFW_SOURCE_FILES})
target_link_libraries(gfw gfw_opengl common)

add_subdirectory(tests)
