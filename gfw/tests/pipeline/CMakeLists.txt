project( GFW_PIPELINE_TESTS )
cmake_minimum_required( VERSION 2.8 )

# Source files

    file( GLOB GFW_PIPELINE_TESTS_SOURCE_FILES *.cpp )
    source_group( "source" FILES ${GFW_PIPELINE_TESTS_SOURCE_FILES} )

    source_group( "fx" FILES ${GFW_TESTS_FX_FILES} )

# Includes

    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source
    )

# Target

    add_executable( gfw_pipeline-tests ${GFW_PIPELINE_TESTS_SOURCE_FILES} ${GFW_TESTS_FX_FILES} )
    target_link_libraries( gfw_pipeline-tests gfw_pipeline )

# Dependencies

    find_package( cmn CONFIG )
    target_link_libraries( gfw_pipeline-tests ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )

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
