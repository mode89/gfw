project(GFW)
cmake_minimum_required(VERSION 2.8)

file(GLOB SOURCE_FILES source/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})
file(GLOB SOURCE_FILES source/graphics/common/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})
file(GLOB SOURCE_FILES source/graphics/opengl/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})
file(GLOB SOURCE_FILES source/platform/win/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})

add_library(gfw ${GFW_SOURCE_FILES})
include_directories(include)

find_package(opengl CONFIG)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(common CONFIG)
target_link_libraries(gfw ${COMMON_LIBRARIES})
include_directories(${COMMON_INCLUDE_DIRS})

find_package(profiler CONFIG)
target_link_libraries(gfw ${PROFILER_LIBRARIES})
include_directories(${PROFILER_INCLUDE_DIRS})

add_subdirectory(tests)
