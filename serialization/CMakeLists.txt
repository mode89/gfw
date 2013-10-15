project( SERIALIZATION )
cmake_minimum_required( VERSION 2.8 )

file( GLOB TEMP_LIST include/serialization/*.h )
list( APPEND SERIALIZATION_INCLUDE_FILES ${TEMP_LIST} )
source_group( "include" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/*.cpp )
list( APPEND SERIALIZATION_SOURCE_FILES ${TEMP_LIST} )
source_group( "source" FILES ${TEMP_LIST} )

add_library( serialization ${SERIALIZATION_SOURCE_FILES} ${SERIALIZATION_INCLUDE_FILES} )
include_directories( include/serialization )

add_subdirectory( tests )
