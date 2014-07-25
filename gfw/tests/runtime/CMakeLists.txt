project( GFW_RUNTIME_TESTS )
cmake_minimum_required( VERSION 2.8 )

file( GLOB TEMP_LIST *.cpp )
source_group( "source" FILES ${TEMP_LIST} )
list( APPEND GFW_RUNTIME_TESTS_SOURCE_FILES ${TEMP_LIST} )

source_group( "fx" FILES ${GFW_TESTS_FX_FILES} )

add_executable( gfw_runtime-tests ${GFW_RUNTIME_TESTS_SOURCE_FILES} ${GFW_TESTS_FX_FILES} )
include_directories( ${GFW_ROOT}/include/base ${GFW_ROOT}/include/shared )

target_link_libraries( gfw_runtime-tests gfw_runtime )

find_package( cmn CONFIG )
target_link_libraries( gfw_runtime-tests ${COMMON_LIBS} )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( gtest CONFIG )
target_link_libraries( gfw_runtime-tests ${GTEST_LIBS} )
include_directories( ${GTEST_INCLUDE_DIRS} )

# Building effects

add_custom_command(
    TARGET gfw_runtime-tests
    POST_BUILD
    COMMAND gfw_fxc -o "${GFW_RUNTIME_TESTS_BINARY_DIR}/draw.fxc" -fx "${GFW_TESTS_DATA_DIR}/draw.fx"
    COMMENT "Building draw.fx" )
