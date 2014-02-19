project(GFW_TESTS)
cmake_minimum_required(VERSION 2.8)

set( GFW_TESTS_DATA_DIR ${GFW_TESTS_SOURCE_DIR}/data/ )
add_definitions( -DTESTS_DATA_DIR="${GFW_TESTS_DATA_DIR}/" )

file( GLOB GFW_TESTS_FX_FILES data/*.fx )

add_subdirectory( runtime )
add_subdirectory( pipeline )
