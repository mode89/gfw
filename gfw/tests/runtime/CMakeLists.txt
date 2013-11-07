project( GFW_RUNTIME_TESTS )
cmake_minimum_required( VERSION 2.8 )

add_definitions( -DTESTS_SOURCE_DIR="${GFW_RUNTIME_TESTS_SOURCE_DIR}/" )

file( GLOB GFW_RUNTIME_TESTS_SOURCE_FILES *.cpp )
add_executable( gfw_runtime-tests ${GFW_RUNTIME_TESTS_SOURCE_FILES} )
include_directories( ${GFW_ROOT}/include/base ${GFW_ROOT}/include/shared )

target_link_libraries( gfw_runtime-tests gfw_runtime )

find_package( common CONFIG )
target_link_libraries( gfw_runtime-tests ${COMMON_LIBRARIES} )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( gtest CONFIG )
target_link_libraries( gfw_runtime-tests ${GTEST_LIBRARIES} )
include_directories( ${GTEST_INCLUDE_DIRS} )

# Building effects

add_custom_command(
    TARGET gfw_runtime-tests
    POST_BUILD
    COMMAND gfw_fxc -o "${GFW_RUNTIME_TESTS_BINARY_DIR}/draw.fxc" -fx "${GFW_RUNTIME_TESTS_SOURCE_DIR}/draw.fx"
    COMMENT Building draw.fx )
