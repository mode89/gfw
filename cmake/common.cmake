# Setup common packages search path

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Static link MinGW std libs

if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# Enable C++11

if (MINGW)
    add_definitions(-std=gnu++11)
endif()

# VS2012 doesn't support correctly the tuples yet

if (MSVC)
    add_definitions(/D _VARIADIC_MAX=10)
endif()

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
