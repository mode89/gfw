project( GFW_PIPELINE )
cmake_minimum_required( VERSION 2.8 )

file( GLOB TEMP_LIST ${GFW_ROOT}/source/pipeline/common/*.cpp )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )
source_group( "source\\common" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/source/pipeline/${GFW_PLATFORM}/*.cpp )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )
source_group( "source\\${GFW_PLATFORM}" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/pipeline/gfw/pipeline/common/*.h )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\common" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/pipeline/gfw/pipeline/${GFW_PLATFORM}/gfw/pipeline/*.h )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\${GFW_PLATFORM}" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/shared/gfw/shared/*.h )
list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\shared" FILES ${TEMP_LIST} )

add_library( gfw_pipeline ${GFW_PIPELINE_SOURCE_FILES} )
include_directories(
    ${GFW_ROOT}/include/pipeline
    ${GFW_ROOT}/include/pipeline/gfw/pipeline/${GFW_PLATFORM}
    ${GFW_PIPELINE_BINARY_DIR}
    ${GFW_ROOT}/include/shared
    )

find_package( antlr CONFIG )
target_link_libraries( gfw_pipeline ${ANTLR_LIBRARIES} )
include_directories( ${ANTLR_INCLUDE_DIRS} )

find_package( common CONFIG )
target_link_libraries( gfw_pipeline ${COMMON_LIBRARIES} )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( serialization CONFIG )
include_directories( ${SERIALIZATION_INCLUDE_DIRS} )

# Pre-build

set( GFW_PIPELINE_FX_GRAMMAR_FILE ${GFW_PIPELINE_SOURCE_DIR}/common/FX.g )
add_custom_command(
    OUTPUT FXLexer.c FXParser.c
    COMMAND java -jar ${ANTLR_TOOL} -make -language C ${GFW_PIPELINE_FX_GRAMMAR_FILE}
    DEPENDS ${GFW_PIPELINE_FX_GRAMMAR_FILE} )
add_custom_target( gfw_pipeline_parser ALL
    DEPENDS FXLexer.c FXParser.c
    SOURCES ${GFW_PIPELINE_FX_GRAMMAR_FILE} )
add_dependencies( gfw_pipeline gfw_pipeline_parser )

# Allow to platform override build process

include( ${GFW_ROOT}/scripts/${GFW_PLATFORM}/build.cmake )
