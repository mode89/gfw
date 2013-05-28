project(GLOBAL_SOLUTION)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DPROFILING=1)

# Include common utils

include(cmake/common.cmake)

# Add all projects to the build tree

file(GLOB SUB_DIRS *)
foreach(DIR ${SUB_DIRS})
    if(IS_DIRECTORY ${DIR})
        if(EXISTS ${DIR}/CMakeLists.txt)
            add_subdirectory(${DIR})
        endif()
    endif()
endforeach()
