file( TO_CMAKE_PATH $ENV{BOOST_ROOT} BOOST_ROOT )

set( BOOST_INCLUDE_DIRS ${BOOST_ROOT} )

set( BOOST_LIBS
    boost-wave
    boost-serialization
    )

set( BOOST_DEFINITIONS
    -DBOOST_ALL_NO_LIB # Prevents automatic linking
    )
