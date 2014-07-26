project( MASTER )
cmake_minimum_required( VERSION 2.8 )

# Global configuration

set( GFW_PLATFORM ogl4 )

set( CMN_STATIC_LINK_STD_LIBS true )

include( cmn/scripts/config.cmake )

# Add necessary projects to the build tree

add_subdirectory( ann )
add_subdirectory( antlr )
add_subdirectory( boost )
add_subdirectory( cmn )
add_subdirectory( ga )
add_subdirectory( gfw )
add_subdirectory( gtest )
#add_subdirectory( profiler )
add_subdirectory( ripples )
