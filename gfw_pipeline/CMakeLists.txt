project( GFW_PIPELINE )
cmake_minimum_required( VERSION 2.8 )

if ( NOT GFW_PLATFORM )
    message( FATAL_ERROR "GFW_PLATFORM is not defined." )
endif()

# Common files

file( GLOB TEMP_LIST include/gfw_pipeline/common/*.h )
source_group( "include\\common" ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_INCLUDE_FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/common/*.cpp )
source_group( "source\\common" ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )

# Platform-specific files

file( GLOB TEMP_LIST include/gfw_pipeline/${GFW_PLATFORM}/*.h )
source_group( "include\\${GFW_PLATFORM}" ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_INCLUDE_FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/${GFW_PLATFORM}/*.cpp )
source_group( "source\\${GFW_PLATFORM}" ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )


add_library( gfw_pipeline ${GFW_PIPELINE_SOURCE_FILES} ${GFW_PIPELINE_INCLUDE_FILES} )
include_directories( include/gfw_pipeline )

add_subdirectory( test )
