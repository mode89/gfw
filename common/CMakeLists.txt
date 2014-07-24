project( COMMON )
cmake_minimum_required( VERSION 2.8 )

# Source files

    file( GLOB COMMON_SOURCE_FILES source/*.cpp )
    source_group( "source" FILES ${COMMON_SOURCE_FILES} )

    file( GLOB COMMON_HEADER_FILES include/common/*.h )
    source_group( "include" FILES ${COMMON_HEADER_FILES} )

# Target

    add_library( common ${COMMON_SOURCE_FILES} ${COMMON_HEADER_FILES} )
    include_directories( include )
