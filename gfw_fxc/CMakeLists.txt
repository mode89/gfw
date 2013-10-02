project( GFW_FXC )
cmake_minimum_required( VERSION 2.8 )

file( GLOB GFW_FXC_SOURCE_FILES source/*.cpp )
source_group( "source" ${GFW_FXC_SOURCE_FILES} )

add_executable( gfw_fxc ${GFW_FXC_SOURCE_FILES} )
