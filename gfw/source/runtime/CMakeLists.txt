project( GFW_RUNTIME )

file( GLOB TEMP_LIST ${GFW_ROOT}/source/runtime/common/*.cpp )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "source\\common" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/source/runtime/${GFW_PLATFORM}/*.cpp )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "source\\core" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/source/runtime/win/*.cpp )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "source\\win" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/base/gfw/*.h )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "include" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/base/gfw/base/*.h )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\base" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/runtime/gfw/runtime/common/*.h )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\common" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/runtime/gfw/runtime/common/*.inl )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\common" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/runtime/gfw/runtime/core/${GFW_PLATFORM}/gfw/runtime/core/*.h )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\core" FILES ${TEMP_LIST} )

file( GLOB TEMP_LIST ${GFW_ROOT}/include/shared/gfw/shared/*.h )
list( APPEND GFW_RUNTIME_SOURCE_FILES ${TEMP_LIST} )
source_group( "include\\shared" FILES ${TEMP_LIST} )

add_library( gfw_runtime ${GFW_RUNTIME_SOURCE_FILES} )
include_directories(
    ${GFW_ROOT}/include/base
    ${GFW_ROOT}/include/runtime
    ${GFW_ROOT}/include/runtime/gfw/runtime/core/${GFW_PLATFORM}
    ${GFW_ROOT}/include/shared
    )

find_package( common CONFIG )
target_link_libraries( gfw_runtime ${COMMON_LIBS} )
include_directories( ${COMMON_INCLUDE_DIRS} )

find_package( serialization CONFIG )
include_directories( ${SERIALIZATION_INCLUDE_DIRS} )

include( ${GFW_ROOT}/scripts/${GFW_PLATFORM}/build.cmake )
