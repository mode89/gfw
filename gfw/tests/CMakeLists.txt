project(GFW_TESTS)
cmake_minimum_required(VERSION 2.8)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()

configure_file(
    ${GFW_TESTS_SOURCE_DIR}/config.h.in
    ${GFW_TESTS_SOURCE_DIR}/config.h
)

add_executable(gfw_test tests.cpp)
target_link_libraries(gfw_test gfw gtest)
