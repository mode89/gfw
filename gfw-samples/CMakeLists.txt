project( GFW_SAMPLES )

file( GLOB TEMP_LIST source/*.cpp )
list( APPEND GFW_SAMPLES_SOURCE_FILES ${TEMP_LIST} )
source_group( "source" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/*.h )
list( APPEND GFW_SAMPLES_SOURCE_FILES ${TEMP_LIST} )
source_group( "include" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST data/*.fx )
list( APPEND GFW_SAMPLES_SOURCE_FILES ${TEMP_LIST} )
source_group( "data" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/common/*.cpp )
list( APPEND GFW_SAMPLES_SOURCE_FILES ${TEMP_LIST} )
source_group( "source\\common" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST source/common/*.h )
list( APPEND GFW_SAMPLES_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\common" FILES ${TEMP_LIST} )

add_executable( gfw-samples ${GFW_SAMPLES_SOURCE_FILES} )
include_directories( source )

find_package( gfw CONFIG )
target_link_libraries( gfw-samples ${GFW_LIBS} )
include_directories( ${GFW_INCLUDE_DIRS} )

file( GLOB GFW_SAMPLES_FX_FILES data/*.fx )
foreach( fxFile ${GFW_SAMPLES_FX_FILES} )
    get_filename_component( fxFileAbsolute ${fxFile} ABSOLUTE )
    get_filename_component( fxFileName ${fxFile} NAME )
    add_custom_command(
        TARGET gfw-samples POST_BUILD
        COMMAND gfw_fxc -o "${GFW_SAMPLES_BINARY_DIR}/${fxFileName}" -fx "fxFileAbsolute"
        COMMENT Building ${fxFileName} )
endforeach( fxFile )
