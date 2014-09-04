project( GFW_GFWC )

# Source files

    file( GLOB TEMP_LIST *.cpp )
    list( APPEND GFW_GFWC_SOURCE_FILES ${TEMP_LIST} )
    source_group( "source" FILES ${TEMP_LIST} )

# Include

    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source
    )

# Target

    add_executable( gfwc ${GFW_GFWC_SOURCE_FILES} )

# Dependencies

    target_link_libraries( gfwc
        gfw-pipeline-fx
        gfw-pipeline-scene )

    find_package( cmn CONFIG )
    include_directories( ${CMN_INCLUDE_DIRS} )

    find_package( boost CONFIG )
    target_link_libraries( gfwc
        boost-filesystem
        boost-serialization
        boost-system
        )
    include_directories( ${BOOST_INCLUDE_DIRS} )
    add_definitions( ${BOOST_DEFINITIONS} )
