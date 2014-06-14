project(THREADPOOL_TESTS)
cmake_minimum_required(VERSION 2.8)

add_executable(threadpool-tests tests.cpp)

find_package(threadpool CONFIG)
target_link_libraries(threadpool-tests ${THREADPOOL_LIBS})
include_directories(${THREADPOOL_INCLUDE_DIRS})

find_package(gtest CONFIG)
target_link_libraries(threadpool-tests ${GTEST_LIBS})
include_directories(${GTEST_INCLUDE_DIRS})
