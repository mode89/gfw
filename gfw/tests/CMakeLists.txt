project(GFW_TESTS)
cmake_minimum_required(VERSION 2.8)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()

add_definitions(-DTESTS_SOURCE_DIR="${GFW_TESTS_SOURCE_DIR}/")

add_executable(gfw_tests tests.cpp)
target_link_libraries(gfw_tests gfw gtest)
