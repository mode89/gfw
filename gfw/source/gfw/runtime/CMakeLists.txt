project( GFW_RUNTIME )

# Source files

    file( GLOB TEMP_LIST ${GFW_ROOT}/include/gfw/*.* )
    list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
    source_group( "base" FILES ${TEMP_LIST} )

    file( GLOB TEMP_LIST common/*.* )
    list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
    source_group( "common" FILES ${TEMP_LIST} )

    file( GLOB TEMP_LIST ${GFW_PLATFORM}/*.* )
    list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
    source_group( "${GFW_PLATFORM}" FILES ${TEMP_LIST} )

# Includes

    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source
    )

# Target

    add_library( gfw-runtime ${GFW_RUNTIME_SOURCE_FILES} )

# Dependencies

    target_link_libraries( gfw-runtime gfw-shared )

    find_package( cmn CONFIG )
    target_link_libraries( gfw-runtime ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )

    find_package( boost CONFIG )
    target_link_libraries( gfw-runtime boost-serialization )
    include_directories( ${BOOST_INCLUDE_DIRS} )
    add_definitions( ${BOOST_DEFINITIONS} )

# Platform-specific build script

    include( ${GFW_PLATFORM}/build.cmake )
