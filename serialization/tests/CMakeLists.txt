project( SERIALIZATION_TESTS )
cmake_minimum_required( VERSION 2.8 )

file( GLOB SERIALIZATION_TESTS_SOURCE_FILES *.cpp )
source_group( "source" FILES ${SERIALIZATION_TESTS_SOURCE_FILES} )

add_executable( serialization-tests ${SERIALIZATION_TESTS_SOURCE_FILES} )
target_link_libraries( serialization-tests serialization )

find_package( gtest CONFIG )
include_directories( ${GTEST_INCLUDE_DIRS} )
target_link_libraries( serialization-tests ${GTEST_LIBRARIES} )
