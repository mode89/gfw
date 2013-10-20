project( GFW_RUNTIME_TESTS )
cmake_minimum_required( VERSION 2.8 )

add_definitions( -DTESTS_SOURCE_DIR="${GFW_RUNTIME_TESTS_SOURCE_DIR}/" )

file( GLOB GFW_RUNTIME_TESTS_SOURCE_FILES *.cpp )
add_executable( gfw_runtime-tests ${GFW_RUNTIME_TESTS_SOURCE_FILES} )

target_link_libraries( gfw_runtime-tests gfw_runtime )

find_package(gtest CONFIG)
target_link_libraries(gfw_runtime-tests ${GTEST_LIBRARIES})
include_directories(${GTEST_INCLUDE_DIRS})
