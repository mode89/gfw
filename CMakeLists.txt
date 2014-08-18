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

    # png

        add_library( png
            png/png.h
            png/pngconf.h
            ${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h
            png/pngdebug.h
            png/pnginfo.h
            png/pngpriv.h
            png/pngstruct.h
            png/png.c
            png/pngerror.c
            png/pngget.c
            png/pngmem.c
            png/pngpread.c
            png/pngread.c
            png/pngrio.c
            png/pngrtran.c
            png/pngrutil.c
            png/pngset.c
            png/pngtrans.c
            png/pngwio.c
            png/pngwrite.c
            png/pngwtran.c
            png/pngwutil.c
        )
        target_link_libraries( png zlib )

        if( MSVC )
            set_property( TARGET png APPEND PROPERTY COMPILE_OPTIONS
                /wd4255 /wd4574 /wd4996 )
        endif()

        set_property( TARGET png APPEND PROPERTY INCLUDE_DIRECTORIES
            ${CMAKE_CURRENT_SOURCE_DIR}/png
            ${CMAKE_CURRENT_BINARY_DIR}
            ${ZLIB_INCLUDE_DIRS}
        )

        configure_file( png/scripts/pnglibconf.h.prebuilt
            ${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h )

        set( PNG_INCLUDE_DIRS
            ${CMAKE_CURRENT_LIST_DIR}/png
            ${CMAKE_CURRENT_BINARY_DIR} )
        set( PNG_LIBS png )

add_subdirectory( ann )
add_subdirectory( antlr )
add_subdirectory( boost )
add_subdirectory( cmn )
add_subdirectory( ga )
add_subdirectory( gfw )
add_subdirectory( gtest )
#add_subdirectory( profiler )
#add_subdirectory( ripples )
