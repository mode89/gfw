project( GFW_SB )
cmake_minimum_required( VERSION 2.8 )

# Source files

    file( GLOB GFW_SB_SOURCE_FILES *.cpp *.h )

# Includes

    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source )

# Target

    add_executable( gfw-sb ${GFW_SB_SOURCE_FILES} )

# Dependencies

    target_link_libraries( gfw-sb gfw-pipeline-scene )

    find_package( boost CONFIG )
    target_link_libraries( gfw-sb
        boost-filesystem
        boost-serialization
        boost-system )
    include_directories( ${BOOST_INCLUDE_DIRS} )
    add_definitions( ${BOOST_DEFINITIONS} )

    find_package( cmn CONFIG )
    target_link_libraries( gfw-sb ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )
