project(GFW)
cmake_minimum_required(VERSION 2.8)

set(GFW_CORE_DEFAULT ogl4)

if (NOT GFW_CORE)
    message(WARNING "GFW_CORE not defined. Using default graphics core implementation. Default is '${GFW_CORE_DEFAULT}'")
    set(GFW_CORE ${GFW_CORE_DEFAULT})
endif()

file(GLOB SOURCE_FILES source/common/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})
file(GLOB SOURCE_FILES source/${GFW_CORE}/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})
file(GLOB SOURCE_FILES source/win/*.cpp)
list(APPEND GFW_SOURCE_FILES ${SOURCE_FILES})

add_library(gfw ${GFW_SOURCE_FILES})
include_directories(include include/gfw/${GFW_CORE})

find_package(common CONFIG)
target_link_libraries(gfw ${COMMON_LIBRARIES})
include_directories(${COMMON_INCLUDE_DIRS})

find_package(profiler CONFIG)
target_link_libraries(gfw ${PROFILER_LIBRARIES})
include_directories(${PROFILER_INCLUDE_DIRS})

include(scripts/${GFW_CORE}/build.cmake)

add_subdirectory(tests)
