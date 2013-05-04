project(COMMON_TESTS)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DTESTS_SOURCE_DIR="${COMMON_TESTS_SOURCE_DIR}/")

file(GLOB COMMON_TESTS_SOURCE_FILES *.cpp)
add_executable(common_tests ${COMMON_TESTS_SOURCE_FILES})
target_link_libraries(common_tests common gtest)
