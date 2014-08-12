find_package(opengl CONFIG)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package( boost CONFIG )
target_link_libraries( gfw_pipeline boost-serialization )
include_directories( ${BOOST_INCLUDE_DIRS} )
add_definitions( ${BOOST_DEFINITIONS} )
