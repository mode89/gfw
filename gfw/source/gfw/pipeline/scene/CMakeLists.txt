project( GFW_PIPELINE_SCENE )
cmake_minimum_required( VERSION 2.8 )

# Source files

    file( GLOB TEMP_LIST common/*.cpp common/*.h )
    list( APPEND GFW_PIPELINE_SCENE_SOURCE_FILES ${TEMP_LIST} )
    source_group( "common" FILES ${TEMP_LIST} )

    # Collada
    file( GLOB TEMP_LIST collada/*.cpp collada/*.h )
    list( APPEND GFW_PIPELINE_SCENE_SOURCE_FILES ${TEMP_LIST} )
    source_group( "collada" FILES ${TEMP_LIST} )

# Include

    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source
        ${CMAKE_CURRENT_BINARY_DIR}
    )

# Target

    add_library( gfw-pipeline-scene ${GFW_PIPELINE_SCENE_SOURCE_FILES} )

# Dependencies

    target_link_libraries( gfw-pipeline-scene gfw-shared )

    find_package( cmn CONFIG )
    target_link_libraries( gfw-pipeline-scene ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )

    find_package( boost CONFIG )
    include_directories( ${BOOST_INCLUDE_DIRS} )
    add_definitions( ${BOOST_DEFINITIONS} )

    # Collada

        target_link_libraries( gfw-pipeline-scene ${TINYXML2_LIBS} )
        include_directories( ${TINYXML2_INCLUDE_DIRS} )
