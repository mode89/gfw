project( BOOST )
cmake_minimum_required( VERSION 2.8 )

if ( "$ENV{BOOST_ROOT}" STREQUAL "" )
    message( WARNING "Environment variable BOOST_ROOT is undefined." )
    set( BOOST_FOUND false )
else()
    set( BOOST_FOUND true )
endif()

if ( BOOST_FOUND )
    file( TO_CMAKE_PATH $ENV{BOOST_ROOT} BOOST_ROOT )

    include_directories( ${BOOST_ROOT} )

    add_definitions( -DBOOST_ALL_NO_LIB )

    # Boost.Filesystem

    file( GLOB_RECURSE BOOST_FILESYSTEM_SOURCE_FILES ${BOOST_ROOT}/libs/filesystem/src/*.cpp )
    add_library( boost-filesystem ${BOOST_FILESYSTEM_SOURCE_FILES} )

    # Boost.Serialization

    file( GLOB_RECURSE BOOST_SERIALIZATION_SOURCE_FILES ${BOOST_ROOT}/libs/serialization/src/*.cpp )
    add_library( boost-serialization ${BOOST_SERIALIZATION_SOURCE_FILES} )

    # Boost.System

    file( GLOB_RECURSE BOOST_SYSTEM_SOURCE_FILES ${BOOST_ROOT}/libs/system/src/*.cpp )
    add_library( boost-system ${BOOST_SYSTEM_SOURCE_FILES} )

    # Boost.Wave

    file( GLOB_RECURSE BOOST_WAVE_SOURCE_FILES ${BOOST_ROOT}/libs/wave/src/*.cpp )
    add_library( boost-wave ${BOOST_WAVE_SOURCE_FILES} )

else()
    message( WARNING "Failed to find Boost." )
endif()
