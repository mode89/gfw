project( GFW_SHARED )
cmake_minimum_required( VERSION 2.8 )

# Source files

    file( GLOB TEMP_LIST *.cpp *.h )

# Target

    add_library( gfw-shared ${TEMP_LIST} )
    include_directories(
        ${GFW_ROOT}/include
        ${GFW_ROOT}/source
    )

# Dependencies

    find_package( cmn CONFIG )
    target_link_libraries( gfw-shared ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )
