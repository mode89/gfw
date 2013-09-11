project(TASKMAN)
cmake_minimum_required(VERSION 2.8)

file(GLOB TEMP_FILES_LIST source/*.cpp)
list(APPEND TASKMAN_SOURCE_FILES ${TEMP_FILES_LIST})
source_group("source" FILES ${TEMP_FILES_LIST})

file(GLOB TEMP_FILES_LIST source/win/*.cpp)
list(APPEND TASKMAN_SOURCE_FILES ${TEMP_FILES_LIST})
source_group("source\\win" FILES ${TEMP_FILES_LIST})

file(GLOB TEMP_FILES_LIST include/taskman/*.h)
list(APPEND TASKMAN_HEADER_FILES ${TEMP_FILES_LIST})
source_group("include" FILES ${TEMP_FILES_LIST})

file(GLOB TEMP_FILES_LIST include/taskman/win/*.h)
list(APPEND TASKMAN_HEADER_FILES ${TEMP_FILES_LIST})
source_group("include\\win" FILES ${TEMP_FILES_LIST})

add_library(taskman ${TASKMAN_SOURCE_FILES} ${TASKMAN_HEADER_FILES})
include_directories(include include/taskman)

find_package(common CONFIG)
target_link_libraries(taskman ${COMMON_LIBRARIES})
include_directories(${COMMON_INCLUDE_DIRS})

add_subdirectory(tests)
