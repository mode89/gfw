project( MASTER )
cmake_minimum_required( VERSION 2.8 )

# Include common utils

include( cmake/common.cmake )

# Global configuration

set( GFW_PLATFORM ogl4 )

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
