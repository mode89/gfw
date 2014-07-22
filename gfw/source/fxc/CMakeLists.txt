project( GFW_FXC )

file( GLOB TEMP_LIST ${GFW_ROOT}/source/fxc/*.cpp )
list( APPEND GFW_FXC_SOURCE_FILES ${TEMP_LIST} )
source_group( "source" FILES ${TEMP_LIST} )

add_executable( gfw_fxc ${GFW_FXC_SOURCE_FILES} )
target_link_libraries( gfw_fxc gfw_pipeline )
include_directories(
    ${GFW_ROOT}/include/base
    ${GFW_ROOT}/include/pipeline
    ${GFW_ROOT}/include/shared
    )

find_package( common CONFIG )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( boost CONFIG )
target_link_libraries( gfw_fxc
    boost-filesystem
    boost-serialization
    boost-system
    )
include_directories( ${BOOST_INCLUDE_DIRS} )
add_definitions( ${BOOST_DEFINITIONS} )
