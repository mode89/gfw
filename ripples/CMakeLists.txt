project( RIPPLES )
cmake_minimum_required( VERSION 2.8 )

add_definitions( -DRIPPLES_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

file( GLOB RIPPLES_SOURCE_FILES source/*.cpp )
add_executable( ripples ${RIPPLES_SOURCE_FILES} )
include_directories( include )

find_package( cmn CONFIG )
target_link_libraries( ripples ${COMMON_LIBS} )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( gfw CONFIG )
target_link_libraries( ripples ${GFW_LIBS} )
include_directories( ${GFW_INCLUDE_DIRS} )

file( GLOB RIPPLES_GLSL_FILES data/*.fx )
foreach ( item ${RIPPLES_GLSL_FILES} )
    add_custom_command(
        TARGET ripples POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${item}" "${CMAKE_CURRENT_BINARY_DIR}"
        )
endforeach()
