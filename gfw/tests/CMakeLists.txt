project(GFW_TESTS)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DTESTS_SOURCE_DIR="${GFW_TESTS_SOURCE_DIR}/")

file(GLOB GFW_TESTS_SOURCE_FILES *.cpp)
add_executable(gfw-tests ${GFW_TESTS_SOURCE_FILES})

find_package(gfw CONFIG)
target_link_libraries(gfw-tests ${GFW_LIBRARIES})
include_directories(${GFW_INCLUDE_DIRS})

find_package(gtest CONFIG)
target_link_libraries(gfw-tests ${GTEST_LIBRARIES})
include_directories(${GTEST_INCLUDE_DIRS})
