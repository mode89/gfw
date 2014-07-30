project( GFW_FXC )

# Source files

    file( GLOB TEMP_LIST *.cpp )
    list( APPEND GFW_FXC_SOURCE_FILES ${TEMP_LIST} )
    source_group( "source" FILES ${TEMP_LIST} )

# Include

    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source
    )

# Target

    add_executable( gfw_fxc ${GFW_FXC_SOURCE_FILES} )
    target_link_libraries( gfw_fxc gfw_pipeline )

# Dependencies

    find_package( cmn CONFIG )
    include_directories( ${CMN_INCLUDE_DIRS} )

    find_package( boost CONFIG )
    target_link_libraries( gfw_fxc
        boost-filesystem
        boost-serialization
        boost-system
        )
    include_directories( ${BOOST_INCLUDE_DIRS} )
    add_definitions( ${BOOST_DEFINITIONS} )
