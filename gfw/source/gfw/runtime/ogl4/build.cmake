find_package( opengl CONFIG )
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries( gfw-runtime ${OPENGL_LIBS} )
