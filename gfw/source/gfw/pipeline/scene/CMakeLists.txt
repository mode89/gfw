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

    find_package( cmn CONFIG )
    target_link_libraries( gfw-pipeline-scene ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )

    # Collada

        target_link_libraries( gfw-pipeline-scene ${TINYXML2_LIBS} )
        include_directories( ${TINYXML2_INCLUDE_DIRS} )
