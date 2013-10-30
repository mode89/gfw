project( GFW )

if ( NOT GFW_PLATFORM )
    message( FATAL_ERROR "GFW_PLATFORM is not defined." )
endif()

set( GFW_ROOT ${CMAKE_CURRENT_LIST_DIR} )

add_subdirectory( source/fxc )
add_subdirectory( source/pipeline )
add_subdirectory( source/runtime )
add_subdirectory( tests )
