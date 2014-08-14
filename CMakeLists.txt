project( MASTER )
cmake_minimum_required( VERSION 2.8 )

# Global configuration

set( GFW_PLATFORM ogl4 )

set( CMN_STATIC_LINK_STD_LIBS true )
set( CMN_ENABLE_CXX11 true )
set( CMN_MULTITHREADED_BUILD true )
set( CMN_MORE_WARNINGS true )
set( CMN_MSVC_DEFINE_VARIADIC_MAX 10 ) # MSVC doesn't support tuples

include( cmn/scripts/config.cmake )

# Add necessary projects to the build tree

    # zlib

        file( GLOB ZLIB_SOURCE_FILES zlib/*.c zlib/*.h )
        add_library( zlib ${ZLIB_SOURCE_FILES} )

        if( MSVC )
            set_property( TARGET zlib APPEND PROPERTY COMPILE_OPTIONS
                /wd4127 /wd4131 /wd4242 /wd4244 /wd4255 /wd4996 )
        endif()

        set_property( TARGET zlib APPEND PROPERTY INCLUDE_DIRECTORIES
            ${CMAKE_CURRENT_SOURCE_DIR}/zlib
            ${CMAKE_CURRENT_BINARY_DIR}
        )

        configure_file(	zlib/zconf.h.cmakein ${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY )

        set( ZLIB_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/zlib )
        set( ZLIB_LIBS zlib )

add_subdirectory( ann )
add_subdirectory( antlr )
add_subdirectory( boost )
add_subdirectory( cmn )
add_subdirectory( ga )
add_subdirectory( gfw )
add_subdirectory( gtest )
#add_subdirectory( profiler )
#add_subdirectory( ripples )
