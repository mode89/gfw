# Setup common packages search path

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Enable multithreaded compiling

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

macro(target_precompiled_header target precHeader precSource)
    if (MSVC_IDE)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/Yu${precHeader}")
        set_source_files_properties(${precSource} PROPERTIES COMPILE_FLAGS "/Yc${precHeader}")
    endif()
endmacro(target_precompiled_header)
