project(GFW_TESTS)
cmake_minimum_required(VERSION 2.8)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()

add_definitions(-DTESTS_SOURCE_DIR="${GFW_TESTS_SOURCE_DIR}/")

add_executable(gfw-tests tests.cpp)

find_package(gfw CONFIG)
target_link_libraries(gfw-tests ${GFW_LIBRARIES})
include_directories(${GFW_INCLUDE_DIRS})

find_package(gtest CONFIG)
target_link_libraries(gfw-tests ${GTEST_LIBRARIES})
include_directories(${GTEST_INCLUDE_DIRS})
