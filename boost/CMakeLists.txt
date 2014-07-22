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

    # Boost.Wave

    file( GLOB_RECURSE BOOST_WAVE_SOURCE_FILES ${BOOST_ROOT}/libs/wave/src/*.cpp )
    add_library( boost-wave ${BOOST_WAVE_SOURCE_FILES} )

    # Boost.Serialization

    file( GLOB_RECURSE BOOST_SERIALIZATION_SOURCE_FILES ${BOOST_ROOT}/libs/serialization/src/*.cpp )
    add_library( boost-serialization ${BOOST_SERIALIZATION_SOURCE_FILES} )
else()
    message( WARNING "Failed to find Boost." )
endif()
