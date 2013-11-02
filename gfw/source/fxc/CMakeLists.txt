project( GFW_FXC )

file( GLOB TEMP_LIST ${GFW_ROOT}/source/fxc/*.cpp )
list( APPEND GFW_FXC_SOURCE_FILES ${TEMP_LIST} )
source_group( "source" FILES ${TEMP_LIST} )

add_executable( gfw_fxc ${GFW_FXC_SOURCE_FILES} )
target_link_libraries( gfw_fxc gfw_pipeline )
include_directories(
    ${GFW_ROOT}/include/pipeline
    ${GFW_ROOT}/include/shared
    )

find_package( common CONFIG )
target_link_libraries( gfw_fxc ${COMMON_LIBRARIES} )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( serialization CONFIG )
include_directories( ${SERIALIZATION_INCLUDE_DIRS} )
