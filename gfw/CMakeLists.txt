package( GFW )

if ( NOT GFW_PLATFORM )
    message( FATAL_ERROR "GFW_PLATFORM is not defined." )
endif()

package_source_groups(
    source/common   source/common/*.cpp
    source/core     source/${GFW_PLATFORM}/*.cpp
    source/win      source/win/*.cpp
    include         include/gfw/*.h
    include/base    include/gfw/base/*.h
    include/common  include/gfw/common/*.h
    include/core    include/${GFW_PLATFORM}/gfw/core/*.h
    )

package_include_dirs( include include/gfw/${GFW_PLATFORM} )

package_add_library()

package_dependencies(
    common
    profiler
    )

include(scripts/${GFW_PLATFORM}/build.cmake)

add_subdirectory(tests)
