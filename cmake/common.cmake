# Setup common packages search path

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include( cmake/package_utils.cmake )

if (CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND g++ -dumpversion OUTPUT_VARIABLE GXX_VERSION)
endif()

# Static link MinGW std libs

if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# Enable C++11

if (MINGW AND ((GXX_VERSION VERSION_GREATER "4.7.0") OR (GXX_VERSION VERSION_EQUAL "4.7.0")))
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11" )
endif()

# VS2012 doesn't support correctly the tuples yet

if (MSVC)
    add_definitions(/D _VARIADIC_MAX=10)
endif()

# Enable multithreaded compiling

if (MSVC)
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} /MP"   )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
endif()

macro(target_precompiled_header target precHeader precSource)
    if (MSVC_IDE)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/Yu${precHeader}")
        set_source_files_properties(${precSource} PROPERTIES COMPILE_FLAGS "/Yc${precHeader}")
    endif()
endmacro(target_precompiled_header)
