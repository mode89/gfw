project(GLOBAL_SOLUTION)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DPROFILING=1)

if (MSVC)
    # Enable multithreaded compiling
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

include_directories(${GLOBAL_SOLUTION_SOURCE_DIR})

# Add all projects to the build tree

file(GLOB SUB_DIRS *)
foreach(DIR ${SUB_DIRS})
    if(IS_DIRECTORY ${DIR})
        if(EXISTS ${DIR}/CMakeLists.txt)
            add_subdirectory(${DIR})
        endif()
    endif()
endforeach()
