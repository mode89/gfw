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

    # Warnings

        if ( MSVC )
            string( CONCAT CMAKE_CXX_FLAGS
                ${CMAKE_CXX_FLAGS}
                " /wd4061"  # Enumerator 'value' in switch of enum 'enum' is not explicitly handled by a case label
                " /wd4100"  # Unreferenced formal parameter
                " /wd4127"  # Conditional expression is constant
                " /wd4191"  # Unsafe conversion
                " /wd4242"  # Conversion from 'type1' to 'type2'; possible loss of data
                " /wd4244"  # 'return' : conversion from 'type1' to 'type2'; possible loss of data
                " /wd4245"  # 'argument' : conversion from 'type1' to 'type2'; signed/unsigned mismatch
                " /wd4263"  # Member function doesn't override any base class virtual member function
                " /wd4264"  # No override available for virtual member function from base; function is hidden
                " /wd4265"  # Class has virtual functions, but destructor is not virtual
                " /wd4310"  # Cast truncates constant value
                " /wd4365"  # Signed/unsigned mismatch during conversion
                " /wd4512"  # Assignment operator could not be generated
                " /wd4548"  # Expression before comma has no effect; expected expression with side-effect
                " /wd4571"  # catch(...) semantics has changed since VC++ 7.1; structured exceptions are no longer caught
                " /wd4619"  # Disabling warning that doesn't exist
                " /wd4625"  # Copy constuctor could not be generated because a base class copy constructor is not accessible
                " /wd4626"  # Assignment operator could not be generated because a base class assignment operator is not accessible
                " /wd4640"  # Construction of local static object is not thread-safe
                " /wd4702"  # Unreachable code
                " /wd4709"  # Comma operator within array index expression
                " /wd4996"  # Deprecated function
                )
        elseif( MINGW )
            string( CONCAT CMAKE_CXX_FLAGS
                ${CMAKE_CXX_FLAGS}
                " -Wno-reorder"                 # Members must appear in initializer list in the same order as they apear in class
                " -Wno-unknown-pragmas"         # Unrecognized #pragma
                " -Wno-unused-but-set-variable" # Local variable is assigned to, but otherwise unused
                " -Wno-unused-local-typedefs"   # Typedef locally defined in a function is not used
                )
        endif()

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
