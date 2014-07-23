if ( "$ENV{BOOST_ROOT}" STREQUAL "" )
    message( SEND_ERROR "Failed to find Boost. Environment variable BOOST_ROOT is undefined." )
else()
    file( TO_CMAKE_PATH $ENV{BOOST_ROOT} BOOST_ROOT )

    set( BOOST_INCLUDE_DIRS ${BOOST_ROOT} )

    set( BOOST_LIBS
        boost-filesystem
        boost-serialization
        boost-system
        boost-wave
        )

    set( BOOST_DEFINITIONS
        -DBOOST_ALL_NO_LIB # Prevents automatic linking
        )
endif()
