project(ANTLR)
cmake_minimum_required(VERSION 2.8)

# Warnings

    if ( MSVC )
        string( CONCAT CMAKE_C_FLAGS
            ${CMAKE_C_FLAGS}
            " /wd4255"  # 'func' : no function prototype fiven: converting '()' to '(void)'
            " /wd4296"  # 'comparison-operator' : expression is always true
            )
    elseif( MINGW )
        string( CONCAT CMAKE_C_FLAGS
            ${CMAKE_C_FLAGS}
            " -Wno-unknown-pragmas"             # Unrecognized #pragma
            " -Wno-type-limits"                 # Comparison expression is always true
            " -Wno-unused-but-set-variable"     # Local variable is assigned to, but otherwise unused
            " -Wno-unused-function"             # Static function is declared but not defined or non-inline static function is unused
            " -Wno-sign-compare"                # Comparisong between signed and unsigned values
            )
    endif()

# Target

    file( GLOB ANTLR_SOURCE_FILES source/*.c )
    add_library( antlr ${ANTLR_SOURCE_FILES} )
    include_directories( include/antlr )
