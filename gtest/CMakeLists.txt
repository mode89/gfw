project(GTEST)
cmake_minimum_required(VERSION 2.8)

# Warnings

    if ( MSVC )
        add_compile_options(
            /wd4061     # Enumerator 'value' in switch of enum 'enum-name' is not explicitly handled by a case label
            /wd4365     # Signed/unsigned mismatch during conversion
            /wd4571     # catch(...) semantics changed since VC++ 7.1; structured exceptions are no longer caught
            /wd4625     # Copy constructor could not be generated because a base class copy constructor is inaccessible
            /wd4626     # Assignment operator could not be generated because a base class assignment operator is inaccessible
            /wd4640     # Construction of local static object is not thread-safe
        )
    elseif ( MINGW )
        add_compile_options(
            -Wno-unused-variable
        )
    endif()

# Target

    add_library(gtest source/gtest-main.cc source/gtest-all.cc)
    include_directories(include)
