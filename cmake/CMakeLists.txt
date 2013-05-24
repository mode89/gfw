project(CMAKE)
cmake_minimum_required(VERSION 2.8)

macro(target_precompiled_header target precHeader precSource)
    if (MSVC)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/Yu${precHeader}")
        set_source_files_properties(${precSource} PROPERTIES COMPILE_FLAGS "/Yc${precHeader}")
    endif()
endmacro(target_precompiled_header)
