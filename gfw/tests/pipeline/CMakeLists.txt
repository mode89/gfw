project( GFW_PIPELINE_TESTS )
cmake_minimum_required( VERSION 2.8 )

file( GLOB GFW_PIPELINE_TESTS_SOURCE_FILES *.cpp )
source_group( "source" FILES ${GFW_PIPELINE_TESTS_SOURCE_FILES} )

source_group( "fx" FILES ${GFW_TESTS_FX_FILES} )

add_executable( gfw_pipeline-tests ${GFW_PIPELINE_TESTS_SOURCE_FILES} ${GFW_TESTS_FX_FILES} )
target_link_libraries( gfw_pipeline-tests gfw_pipeline )
include_directories(
    ${GFW_ROOT}/include/base
    ${GFW_ROOT}/include/pipeline
    ${GFW_ROOT}/include/pipeline/gfw/pipeline/${GFW_PLATFORM}
    ${GFW_ROOT}/include/shared
    )

find_package( common CONFIG )
target_link_libraries( gfw_pipeline-tests ${COMMON_LIBS} )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( gtest CONFIG )
target_link_libraries( gfw_pipeline-tests ${GTEST_LIBS} )
include_directories( ${GTEST_INCLUDE_DIRS} )

find_package( boost CONFIG )
target_link_libraries( gfw_pipeline-tests
    boost-filesystem
    boost-system
    )
include_directories( ${BOOST_INCLUDE_DIRS} )
add_definitions( ${BOOST_DEFINITIONS} )
