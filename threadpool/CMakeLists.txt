project(THREADPOOL)
cmake_minimum_required(VERSION 2.8)

file(GLOB TEMP_FILES_LIST source/*.cpp)
list(APPEND THREADPOOL_SOURCE_FILES ${TEMP_FILES_LIST})
source_group("source" FILES ${TEMP_FILES_LIST})

file(GLOB TEMP_FILES_LIST source/win/*.cpp)
list(APPEND THREADPOOL_SOURCE_FILES ${TEMP_FILES_LIST})
source_group("source\\win" FILES ${TEMP_FILES_LIST})

file(GLOB TEMP_FILES_LIST include/threadpool/*.h)
list(APPEND THREADPOOL_HEADER_FILES ${TEMP_FILES_LIST})
source_group("include" FILES ${TEMP_FILES_LIST})

file(GLOB TEMP_FILES_LIST include/threadpool/win/*.h)
list(APPEND THREADPOOL_HEADER_FILES ${TEMP_FILES_LIST})
source_group("include\\win" FILES ${TEMP_FILES_LIST})

add_library(threadpool ${THREADPOOL_SOURCE_FILES} ${THREADPOOL_HEADER_FILES})
include_directories(include include/threadpool)

find_package(common CONFIG)
target_link_libraries(threadpool ${COMMON_LIBS})
include_directories(${COMMON_INCLUDE_DIRS})

add_subdirectory(tests)
