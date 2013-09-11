project(TASKMAN_TESTS)
cmake_minimum_required(VERSION 2.8)

add_executable(taskman-tests tests.cpp)

find_package(taskman CONFIG)
target_link_libraries(taskman-tests ${TASKMAN_LIBRARIES})
include_directories(${TASKMAN_INCLUDE_DIRS})

find_package(gtest CONFIG)
target_link_libraries(taskman-tests ${GTEST_LIBRARIES})
include_directories(${GTEST_INCLUDE_DIRS})
