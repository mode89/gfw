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

# Disable CRT warnings in VS

if ( MSVC )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

macro(target_precompiled_header target precHeader precSource)
    if (MSVC_IDE)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/Yu${precHeader}")
        set_source_files_properties(${precSource} PROPERTIES COMPILE_FLAGS "/Yc${precHeader}")
    endif()
endmacro(target_precompiled_header)

function( target_dependencies targetName )
    math( EXPR PACKC "${ARGC} - 1" )
    foreach( ARGN RANGE 1 ${PACKC} )
        set( PACKAGE ${ARGV${ARGN}} )
        find_package( ${PACKAGE} CONFIG )
        string( TOUPPER ${PACKAGE} PACKAGE )
        target_link_libraries( ${targetName} ${${PACKAGE}_LIBS} )
        include_directories( ${${PACKAGE}_INCLUDE_DIRS} )
    endforeach()
endfunction()
