project(COMMON_TESTS)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DTESTS_SOURCE_DIR="${COMMON_TESTS_SOURCE_DIR}/")

file(GLOB COMMON_TESTS_SOURCE_FILES *.cpp)
add_executable(common-tests ${COMMON_TESTS_SOURCE_FILES})

find_package(common CONFIG)
target_link_libraries(common-tests ${COMMON_LIBS})
include_directories(${COMMON_INCLUDE_DIRS})

find_package(gtest CONFIG)
target_link_libraries(common-tests ${GTEST_LIBS})
include_directories(${GTEST_INCLUDE_DIRS})
