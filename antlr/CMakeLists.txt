project(ANTLR)
cmake_minimum_required(VERSION 2.8)

# Warnings

    if ( MSVC )
        string( CONCAT CMAKE_C_FLAGS
            ${CMAKE_C_FLAGS}
            " /wd4255"  # 'func' : no function prototype fiven: converting '()' to '(void)'
            " /wd4296"  # 'comparison-operator' : expression is always true
            )
    endif()

# Target

    file( GLOB ANTLR_SOURCE_FILES source/*.c )
    add_library( antlr ${ANTLR_SOURCE_FILES} )
    include_directories( include/antlr )
