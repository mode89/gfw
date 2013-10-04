project( GFW_PIPELINE )
cmake_minimum_required( VERSION 2.8 )

if ( NOT GFW_PLATFORM )
    message( FATAL_ERROR "GFW_PLATFORM is not defined." )
endif()

# Common files

file( GLOB TEMP_LIST include/common/gfw_pipeline/*.h )
source_group( "include\\common" FILES ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_INCLUDE_FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/common/*.cpp )
source_group( "source\\common" FILES ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )

# Platform-specific files

file( GLOB TEMP_LIST include/${GFW_PLATFORM}/gfw_pipeline/*.h )
source_group( "include\\${GFW_PLATFORM}" FILES ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_INCLUDE_FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/${GFW_PLATFORM}/*.cpp )
source_group( "source\\${GFW_PLATFORM}" FILES ${TEMP_LIST} )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )


add_library( gfw_pipeline ${GFW_PIPELINE_SOURCE_FILES} ${GFW_PIPELINE_INCLUDE_FILES} )
include_directories(
    include/common
    include/${GFW_PLATFORM}
    ${GFW_PIPELINE_BINARY_DIR}
)

find_package( antlr CONFIG )
target_link_libraries( gfw_pipeline ${ANTLR_LIBRARIES} )
include_directories( ${ANTLR_INCLUDE_DIRS} )

find_package( common CONFIG )
target_link_libraries( gfw_pipeline ${COMMON_LIBRARIES} )
include_directories( ${COMMON_INCLUDE_DIRS} )

# Pre-build

antlr_prebuild_grammar( gfw_pipeline ${CMAKE_CURRENT_LIST_DIR}/data/FX.g )

# Tests

add_subdirectory( tests )
