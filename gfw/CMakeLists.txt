project( GFW )

if ( NOT GFW_PLATFORM )
    message( FATAL_ERROR "GFW_PLATFORM is not defined." )
endif()

set( GFW_ROOT ${CMAKE_CURRENT_LIST_DIR} )

add_definitions( -DGFW_PLATFORM_LIMITS_H="gfw/runtime/${GFW_PLATFORM}/limits.h" )

add_subdirectory( source/gfw/fxc )
add_subdirectory( source/gfw/pipeline/fx )
add_subdirectory( source/gfw/pipeline/scene )
add_subdirectory( source/gfw/runtime )
add_subdirectory( source/gfw/shared )
add_subdirectory( tests )
