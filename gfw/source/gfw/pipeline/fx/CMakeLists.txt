project( GFW_PIPELINE )
cmake_minimum_required( VERSION 2.8 )

# Source files

    file( GLOB TEMP_LIST common/*.cpp common/*.h )
    list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )
    source_group( "common" FILES ${TEMP_LIST} )

    file( GLOB TEMP_LIST ${GFW_PLATFORM}/*.cpp ${GFW_PLATFORM}/*.h )
    list( APPEND GFW_PIPELINE_SOURCE_FILES ${TEMP_LIST} )
    source_group( "${GFW_PLATFORM}" FILES ${TEMP_LIST} )

# Include

    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source
        ${CMAKE_CURRENT_BINARY_DIR}
    )

# Target

    add_library( gfw-pipeline-fx ${GFW_PIPELINE_SOURCE_FILES} )

# Dependencies

    target_link_libraries( gfw-pipeline-fx gfw-shared )

    find_package( antlr CONFIG )
    target_link_libraries( gfw-pipeline-fx ${ANTLR_LIBS} )
    include_directories( ${ANTLR_INCLUDE_DIRS} )

    find_package( cmn CONFIG )
    target_link_libraries( gfw-pipeline-fx ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )

    find_package( boost CONFIG )
    include_directories( ${BOOST_INCLUDE_DIRS} )
    add_definitions( ${BOOST_DEFINITIONS} )

# Pre-build

    set( GFW_PIPELINE_FX_GRAMMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/common/FX.g )
    add_custom_command(
        OUTPUT FXLexer.c FXParser.c
        COMMAND java -jar ${ANTLR_TOOL} -make -language C ${GFW_PIPELINE_FX_GRAMMAR_FILE}
        DEPENDS ${GFW_PIPELINE_FX_GRAMMAR_FILE} )
    add_custom_target( gfw_pipeline_parser ALL
        DEPENDS FXLexer.c FXParser.c
        SOURCES ${GFW_PIPELINE_FX_GRAMMAR_FILE} )
    add_dependencies( gfw-pipeline-fx gfw_pipeline_parser )

# Platform-specific build script

    include( ${GFW_PLATFORM}/build.cmake )
