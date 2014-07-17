project( BOOST )
cmake_minimum_required( VERSION 3.0 )

if ( "$ENV{BOOST_ROOT}" STREQUAL "" )
    message( WARNING "Environment variable BOOST_ROOT is undefined." )
    set( BOOST_FOUND false )
else()
    set( BOOST_FOUND true )
endif()

if ( BOOST_FOUND )
    file( TO_CMAKE_PATH $ENV{BOOST_ROOT} BOOST_ROOT )

    include_directories( ${BOOST_ROOT} )
else()
    message( WARNING "Failed to find Boost." )
endif()
