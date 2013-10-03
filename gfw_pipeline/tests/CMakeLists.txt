project( GFW_PIPELINE_TESTS )
cmake_minimum_required( VERSION 2.8 )

add_definitions( -DTESTS_DIR="${GFW_PIPELINE_TESTS_SOURCE_DIR}/" )

file( GLOB GFW_PIPELINE_TESTS_SOURCE_FILES *.cpp )
add_executable( gfw_pipeline-tests ${GFW_PIPELINE_TESTS_SOURCE_FILES} )

find_package( gfw_pipeline CONFIG )
target_link_libraries( gfw_pipeline-tests ${GFW_PIPELINE_LIBRARIES} )
include_directories( ${GFW_PIPELINE_INCLUDE_DIRS} )

find_package( gtest CONFIG )
target_link_libraries( gfw_pipeline-tests ${GTEST_LIBRARIES} )
include_directories( ${GTEST_INCLUDE_DIRS} )
