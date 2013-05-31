project(RIPPLES)
cmake_minimum_required(VERSION 2.8)

file(GLOB RIPPLES_SOURCE_FILES source/*.cpp)
add_executable(ripples ${RIPPLES_SOURCE_FILES})
include_directories(include)
target_precompiled_header(ripples "ripples/stdafx.h" "source/stdafx.cpp")

find_package(ogre CONFIG)
target_link_libraries(ripples ${OGRE_LIBRARIES})
include_directories(${OGRE_INCLUDE_DIRS})
copy_ogre_dlls(ripples)
